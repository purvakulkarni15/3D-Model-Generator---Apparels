#include "ModelGenerator.h"


ModelGenerator::ModelGenerator()
{
	apparel = Apparel();
	clothSimulator = ClothSimulator();
	avatar = Avatar();
}


ModelGenerator::~ModelGenerator()
{
}

void  ModelGenerator::generate2DModel(ImageProcessor imageProcessorObj) {
	generateModelF(imageProcessorObj);
	generateModelB(imageProcessorObj);
}

void  ModelGenerator::generate3DModel(float xScale, float yScale, glm::vec3 center, const char* front, const char* back) {
	apparel.modifyObject(glm::vec3(xScale, yScale, 1.0), glm::vec3(center.x / 2.0, center.y / 2.0, 0));
	clothSimulator.addConstraints(&apparel);
	intersectApparelWithAvatar();
	clothSimulator.satisfyConstraints(1);

	float minZF = 99999.0;
	float minZB = 99999.0;
	float maxZB = -99999.0;
	float maxZF = -99999.0;

	float scaleVal = 1;

	for (int i = 0; i < apparel.particlesF.size(); i++) {
		if (minZF > apparel.particlesF[i].vertex.z) {
			minZF = apparel.particlesF[i].vertex.z;
		}
		if (maxZF < apparel.particlesF[i].vertex.z) {
			maxZF = apparel.particlesF[i].vertex.z;
		}
	}

	for (int i = 0; i < apparel.particlesB.size(); i++) {
		if (minZB > apparel.particlesB[i].vertex.z) {
			minZB = apparel.particlesB[i].vertex.z;
		}
		if (maxZB < apparel.particlesB[i].vertex.z) {
			maxZB = apparel.particlesB[i].vertex.z;
		}
	}

	if (maxZB > minZF) {
		move((glm::abs(maxZB - minZF) / 2.0));
		scaleVal = glm::abs(maxZF - minZB) / (glm::abs(maxZF - minZB)+ glm::abs(maxZB - minZF));

	}

	stitchFrontAndBack();
	//scale(scaleVal);
	clothSimulator.satisfyConstraints(2);
	scale(scaleVal);

	storeModel(front, back);
}

void ModelGenerator::generateModelF(ImageProcessor imageProcessorObj) {

	imageProcessorObj.processFrontImage();
	glm::vec2* boundingBoxF = findBoundingBox(imageProcessorObj.frontImageContourSet);

	apparel.boundingBoxF[0] = glm::vec3(boundingBoxF[0], 0);
	apparel.boundingBoxF[1] = glm::vec3(boundingBoxF[1], 0);


	apparel.bbWidthF = boundingBoxF[1].x - boundingBoxF[0].x + 1;
	apparel.bbHeightF = boundingBoxF[1].y - boundingBoxF[0].y + 1;

	apparel.fHeight = (float)imageProcessorObj.frontImage.rows;
	apparel.fWidth = (float)imageProcessorObj.frontImage.cols;
	apparel.bHeight = (float)imageProcessorObj.backImage.rows;
	apparel.bWidth = (float)imageProcessorObj.backImage.cols;
	apparel.interval = 5;

	addPoints(apparel.boundaryPointsF, apparel.particlesF, imageProcessorObj.frontImageContourSet, boundingBoxF, true);
	
	clothSimulator.setClothMatrixF(&apparel);
	
	Subdiv2D subdiv = triangulate(apparel.particlesF, apparel.bbHeightF, apparel.bbWidthF);
	apparel.setTrianglesF(subdiv, clothSimulator.clothMatrixF);

	for (int i = 0; i < apparel.particlesF.size(); i++) {
		apparel.particlesF[i].vertex.x = apparel.particlesF[i].vertex.x - apparel.boundingBoxF[0].x - ((apparel.boundingBoxF[1].x - apparel.boundingBoxF[0].x) / 2.0);
		apparel.particlesF[i].vertex.y = apparel.particlesF[i].vertex.y - apparel.boundingBoxF[0].y - ((apparel.boundingBoxF[1].y - apparel.boundingBoxF[0].y) / 2.0);
	}

}

void ModelGenerator::generateModelB(ImageProcessor imageProcessorObj) {

	imageProcessorObj.processBackImage();

	glm::vec2* boundingBoxB = findBoundingBox(imageProcessorObj.backImageContourSet);
	apparel.boundingBoxB[0] = glm::vec3(boundingBoxB[0], 0);
	apparel.boundingBoxB[1] = glm::vec3(boundingBoxB[1], 0);

	apparel.bbWidthB = boundingBoxB[1].x - boundingBoxB[0].x + 1;
	apparel.bbHeightB = boundingBoxB[1].y - boundingBoxB[0].y + 1;

	addPoints(apparel.boundaryPointsB, apparel.particlesB, imageProcessorObj.backImageContourSet, boundingBoxB, false);

	clothSimulator.setClothMatrixB(&apparel);

	Subdiv2D subdiv = triangulate(apparel.particlesB, apparel.bbHeightB, apparel.bbWidthB);
	apparel.setTrianglesB(subdiv, clothSimulator.clothMatrixB);


	for (int i = 0; i < apparel.particlesB.size(); i++) {
		apparel.particlesB[i].vertex.x = apparel.particlesB[i].vertex.x - apparel.boundingBoxB[0].x - ((apparel.boundingBoxB[1].x - apparel.boundingBoxB[0].x) / 2.0);
		apparel.particlesB[i].vertex.y = apparel.particlesB[i].vertex.y - apparel.boundingBoxB[0].y - ((apparel.boundingBoxB[1].y - apparel.boundingBoxB[0].y) / 2.0);
	}
}

int ModelGenerator::getParticleIndex(vector<Particle*> &particles, glm::vec2 point) {
	
	int index = -1;

	for (int i = 0; i < particles.size(); i++) {

		glm::vec2 temp = glm::vec2((particles[i]->vertex.x - point.x), (particles[i]->vertex.y - point.y));
		float dist = glm::sqrt(temp.x*temp.x + temp.y*temp.y);
		
		if (dist < 5.0) {
			particles[i]->isStitched = true;
			index = i;
			break;
		}
	}
	return index;
}

void ModelGenerator::setStitchParticles(vector<Particle*> &particles, vector<vector<Particle*>> &stitchingPoints, int start, int end) {

	vector<Particle*> points;

	int i = start;
	while (i != end) {

		particles[i]->isStitched = true;
		points.push_back(particles[i]);
		if (i == particles.size() - 1) {
			i = -1;
		}
		i++;
	}
	points.push_back(particles[end]);

	stitchingPoints.push_back(points);

}

glm::vec2* ModelGenerator::findBoundingBox(vector<vector<Point>> points) {

	glm::vec2* boundingBox = (glm::vec2*)malloc(sizeof(glm::vec2)*2);

	boundingBox[0].x = 9999.0;
	boundingBox[0].y = 9999.0;
	boundingBox[1].x = -9999.0;
	boundingBox[1].y = -9999.0;

	int i = 0;
	//for (int i = 1; i < points.size(); i++) {
		for (int j = 0; j < points[i].size(); j++) {
			if (boundingBox[0].x > points[i][j].x) {
				boundingBox[0].x = points[i][j].x;
			}
			if (boundingBox[0].y > points[i][j].y) {
				boundingBox[0].y = points[i][j].y;
			}
			if (boundingBox[1].x < points[i][j].x) {
				boundingBox[1].x = points[i][j].x;
			}
			if (boundingBox[1].y < points[i][j].y) {
				boundingBox[1].y = points[i][j].y;
			}
		}
	//}

	return boundingBox;
}

void ModelGenerator::addPoints(vector<Particle*> &boundaryPoints, vector<Particle> &particles, vector<vector<Point>> countourSet, glm::vec2* boundingBox, bool isFront) {

	int interval = apparel.interval;
	float width, height;
	int scanLine = 0;

	width = boundingBox[1].x - boundingBox[0].x;
	height = boundingBox[1].y - boundingBox[0].y;

	for (int j = 0; j < countourSet[0].size(); j += interval) {
		
		Particle p = Particle(glm::vec3(countourSet[0][j].x, countourSet[0][j].y, 0));
		p.isEdge = true;
		particles.push_back(p);
	}
	
	for (int i = boundingBox[0].y; i < boundingBox[1].y; i += interval*2) {
		for (int j = boundingBox[0].x; j < boundingBox[1].x; j+= interval*2) {
			Particle p = Particle(glm::vec3((double)j, (double)i, 0));

			if (cv::pointPolygonTest(countourSet[0], Point2f(p.vertex.x, p.vertex.y), true) > 5.0)
			{
				p.isEdge = false;
				p.scanLine = scanLine;
				particles.push_back(p);
				scanLine++;
			}

		}
	}

	for (int i = 0; i < particles.size(); i++) {
		particles[i].vertex.x = particles[i].vertex.x - boundingBox[0].x;
		particles[i].vertex.y = particles[i].vertex.y - boundingBox[0].y;
		if (isFront) {
			particles[i].vertex.z = avatar.boundingBox[1].z + 1.0;
		}
		else {
			particles[i].vertex.z = avatar.boundingBox[0].z - 1.0;
		}
		if (particles[i].isEdge) {
			boundaryPoints.push_back(&particles[i]);
		}
	}
}

Subdiv2D ModelGenerator::triangulate(vector<Particle> particles, int height, int width) {

	vector<cv::Point2f> pointsDelaunayTriangulation;
	Subdiv2D subdiv(cv::Rect(0, 0, width, height));

	for (int i = 0; i < particles.size(); i++)
	{
		pointsDelaunayTriangulation.push_back(cv::Point2f(particles[i].vertex.x, particles[i].vertex.y));
	}

	for (std::vector<cv::Point2f>::iterator it = pointsDelaunayTriangulation.begin(); it != pointsDelaunayTriangulation.end(); it++)
	{
		subdiv.insert(*it);
	}
	
	return subdiv;
}


void ModelGenerator::intersectFrontWithAvatar() {

	RayCaster rayCaster;
	float z = glm::abs(avatar.boundingBox[1].z - avatar.boundingBox[0].z);
	float minZ = 9999.0;
	

	for (int i = 0; i < apparel.particlesF.size(); i++) {
		glm::vec3 intersectionPoint;
		float distance;
		float minDistance = 99999.0;
		Ray ray;
		ray.orig = apparel.particlesF[i].vertex;
		ray.dir = glm::vec3(0, 0, -1.0);
		bool found = false;


		for (int j = 0; j < avatar.triangles.size(); j++) {

			if (rayCaster.isRayTriangleIntersect(avatar.triangles[j], ray, &intersectionPoint, &distance)) {

				if (distance < minDistance) {
					apparel.particlesF[i].vertex = intersectionPoint;
					apparel.particlesF[i].isIntersect = true;
					minDistance = distance;
					found = true;
				}
			}
		}
		if (found) {
			if (glm::abs(apparel.particlesF[i].vertex.z - avatar.boundingBox[1].z) < 30.0) {
				apparel.particlesF[i].isStatic = true;
			}
		}
		if (found && minZ > apparel.particlesF[i].vertex.z) {
			minZ = apparel.particlesF[i].vertex.z;
		}

		if (found == false) {
			apparel.particlesF[i].vertex.z = 9999.0;
		}

	}

	for (int i = 0; i < apparel.particlesF.size(); i++) {
		if (apparel.particlesF[i].vertex.z == 9999.0) {
			apparel.particlesF[i].vertex.z = minZ;
		}
	}


}

void ModelGenerator::intersectBackWithAvatar() {
	
	RayCaster rayCaster;
	float z = glm::abs(avatar.boundingBox[1].z - avatar.boundingBox[0].z);
	float maxZ = -9999.0;


	for (int i = 0; i < apparel.particlesB.size(); i++) {

		glm::vec3 intersectionPoint;
		float distance;
		float minDistance = 99999.0;
		Ray ray;
		ray.orig = apparel.particlesB[i].vertex;
		ray.dir = glm::vec3(0, 0, 1.0);

		bool found = false;

		for (int j = 0; j < avatar.triangles.size(); j++) {

			if (rayCaster.isRayTriangleIntersect(avatar.triangles[j], ray, &intersectionPoint, &distance)) {

				if (minDistance > distance) {
					apparel.particlesB[i].vertex = intersectionPoint;
					apparel.particlesB[i].isIntersect = true;
					minDistance = distance;
					found = true;
				}
			}
		}

		if (found) {
			if (glm::abs(apparel.particlesB[i].vertex.z - avatar.boundingBox[0].z) < 30.0) {
				apparel.particlesB[i].isStatic = true;
			}
		}
		if (found && maxZ < apparel.particlesB[i].vertex.z) {
			maxZ = apparel.particlesB[i].vertex.z;
		}

		if (found == false) {
			apparel.particlesB[i].vertex.z = 9999.0;
		}

	}

	for (int i = 0; i < apparel.particlesB.size(); i++) {
		if (apparel.particlesB[i].vertex.z == 9999.0) {
			apparel.particlesB[i].vertex.z = maxZ;
		}
	}

}

void ModelGenerator::intersectApparelWithAvatar() {

	intersectFrontWithAvatar();
	intersectBackWithAvatar();
}


void ModelGenerator::move(float distance) {



	for (int i = 0; i < apparel.particlesF.size(); i++) {

		apparel.particlesF[i].vertex.z = apparel.particlesF[i].vertex.z + distance;

	}

	for (int i = 0; i < apparel.particlesB.size(); i++) {

		apparel.particlesB[i].vertex.z = apparel.particlesB[i].vertex.z - distance;

	}
}


void ModelGenerator::scale(float scale) {



	for (int i = 0; i < apparel.particlesF.size(); i++) {

		apparel.particlesF[i].vertex.z = apparel.particlesF[i].vertex.z * scale;

	}

	for (int i = 0; i < apparel.particlesB.size(); i++) {

		apparel.particlesB[i].vertex.z = apparel.particlesB[i].vertex.z * scale;

	}
}

void ModelGenerator::stitchFrontAndBack() {

	for (int i = 0; i < apparel.stitchingPointsF.size(); i++) {
		
		int j = 0;
		
		while (j < apparel.stitchingPointsF[i].size()-1 && j < apparel.stitchingPointsB[i].size()-1) {

			glm::vec3 v = apparel.stitchingPointsF[i][j]->vertex + apparel.stitchingPointsB[i][j]->vertex;
			v = glm::vec3(v.x / 2.0, v.y / 2.0, v.z / 2.0);

			apparel.stitchingPointsF[i][j]->vertex = v;
			apparel.stitchingPointsB[i][j]->vertex = v;

			v = apparel.stitchingPointsF[i][j+1]->vertex + apparel.stitchingPointsB[i][j+1]->vertex;
			v = glm::vec3(v.x / 2.0, v.y / 2.0, v.z / 2.0);

			apparel.stitchingPointsF[i][j+1]->vertex = v;
			apparel.stitchingPointsB[i][j+1]->vertex = v;

			

			Triangle t1 = Triangle(apparel.stitchingPointsF[i][j], apparel.stitchingPointsB[i][j], apparel.stitchingPointsF[i][j + 1]);
			Triangle t2 = Triangle(apparel.stitchingPointsF[i][j + 1], apparel.stitchingPointsB[i][j], apparel.stitchingPointsB[i][j + 1]);

			apparel.trianglesB.push_back(t1);
			apparel.trianglesB.push_back(t2);

			j++;
		}

		while (j < apparel.stitchingPointsF[i].size()-1) {
			apparel.stitchingPointsF[i][j]->vertex = apparel.stitchingPointsB[i][apparel.stitchingPointsB[i].size() - 1]->vertex;

			Triangle t1 = Triangle(apparel.stitchingPointsF[i][j], apparel.stitchingPointsB[i][apparel.stitchingPointsB[i].size()-1], apparel.stitchingPointsF[i][j + 1]);	
			apparel.trianglesB.push_back(t1);
			
			j++;
		}

		while (j < apparel.stitchingPointsB[i].size() - 1) {

			apparel.stitchingPointsB[i][j]->vertex = apparel.stitchingPointsF[i][apparel.stitchingPointsF[i].size() - 1]->vertex;

			Triangle t1 = Triangle(apparel.stitchingPointsB[i][j], apparel.stitchingPointsF[i][apparel.stitchingPointsF[i].size() - 1], apparel.stitchingPointsB[i][j + 1]);
			
			apparel.trianglesB.push_back(t1);
			
			j++;
		}

	}
}


void ModelGenerator::storeModel(const char* frontImageFileName, const char* backImageFileName) {

	CreateDirectory(modelDirectory, NULL);
	CopyFile(frontImageFileName, modelTextureFrontImage, false);
	CopyFile(backImageFileName, modelTextureBackImage, false);

	FILE* fp = fopen(modelObj, "w");

	fprintf(fp, "mtllib apparel.mtl\n");

	for (int i = 0; i < apparel.particlesF.size(); i++)
	{
		fprintf(fp, "v %f %f %f\n", apparel.particlesF[i].vertex.x, apparel.particlesF[i].vertex.y, apparel.particlesF[i].vertex.z);
		fprintf(fp, "vt %f %f\n", apparel.particlesF[i].texture.x/apparel.fWidth, 1 - apparel.particlesF[i].texture.y/apparel.fHeight);
	}

	for (int i = 0; i < apparel.particlesB.size(); i++)
	{
		fprintf(fp, "v %f %f %f\n", apparel.particlesB[i].vertex.x, apparel.particlesB[i].vertex.y, apparel.particlesB[i].vertex.z);
		fprintf(fp, "vt %f %f\n", apparel.particlesB[i].texture.x/apparel.bWidth, 1 - apparel.particlesB[i].texture.y/apparel.bHeight);
	}


	fprintf(fp, "usemtl front\n");
	for (int i = 0; i < apparel.trianglesF.size(); i++) {

		int index1 = apparel.trianglesF[i].p1->index;
		int index2 = apparel.trianglesF[i].p2->index;
		int index3 = apparel.trianglesF[i].p3->index;
		 
		fprintf(fp, "f %d/%d %d/%d %d/%d\n", index1, index1, index2, index2, index3, index3);
	}

	fprintf(fp, "usemtl back\n");
	for (int i = 0; i < apparel.trianglesB.size(); i++) {

		int index1 = apparel.trianglesB[i].p1->index;
		int index2 = apparel.trianglesB[i].p2->index;
		int index3 = apparel.trianglesB[i].p3->index;


		fprintf(fp, "f %d/%d %d/%d %d/%d\n", index1, index1, index2, index2, index3, index3);
	}

	fclose(fp);

	FILE* fp2 = fopen(modelMtl, "w");

	fprintf(fp2, "newmtl front\n");
	fprintf(fp2, "map_Ka front.jpg \n");

	fprintf(fp2, "newmtl back\n");
	fprintf(fp2, "map_Ka back.jpg \n");
}