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

void  ModelGenerator::generateModel(ImageProcessor imageProcessorObj) {
	generateModelF(imageProcessorObj);
	generateModelB(imageProcessorObj);
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
	apparel.interval = 8;

	addPoints(apparel.boundaryPointsF, apparel.particlesF, imageProcessorObj.frontImageContourSet, boundingBoxF, true);
	
	clothSimulator.setClothMatrixF(&apparel);
	
	Subdiv2D subdiv = triangulate(apparel.particlesF, apparel.bbHeightF, apparel.bbWidthF);
	apparel.setTrianglesF(subdiv, clothSimulator.clothMatrixF);
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

	width = boundingBox[1].x - boundingBox[0].x;
	height = boundingBox[1].y - boundingBox[0].y;

	for (int j = 0; j < countourSet[0].size(); j += interval) {
		
		Particle p = Particle(glm::vec3(countourSet[0][j].x, countourSet[0][j].y, 0));
		p.isEdge = true;
		particles.push_back(p);
	}

	for (int i = boundingBox[0].y; i < boundingBox[1].y; i+= interval*2) {
		for (int j = boundingBox[0].x; j < boundingBox[1].x; j+= interval*2) {
			Particle p = Particle(glm::vec3((double)j, (double)i, 0));
			if (cv::pointPolygonTest(countourSet[0], Point2f(p.vertex.x, p.vertex.y), true) > 5.0)
			{
				particles.push_back(p);
			}
		}
	}

	for (int i = 0; i < particles.size(); i++) {
		particles[i].vertex.x = particles[i].vertex.x - boundingBox[0].x;
		particles[i].vertex.y = particles[i].vertex.y - boundingBox[0].y;
		if (isFront) {
			particles[i].vertex.z = avatar.boundingBox[1].z;
		}
		else {
			particles[i].vertex.z = avatar.boundingBox[0].z;
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

	for (int i = 0; i < apparel.particlesF.size(); i++) {
		glm::vec3 intersectionPoint;
		float distance;
		Ray ray;
		ray.orig = apparel.particlesF[i].vertex;
		ray.dir = glm::vec3(0, 0, 1.0);
		bool found = false;


		for (int j = 0; j < avatar.triangles.size(); j++) {

			if (rayCaster.isRayTriangleIntersect(avatar.triangles[j], ray, &intersectionPoint, &distance)) {

				apparel.particlesF[i].vertex = intersectionPoint;

				if (apparel.particlesF[i].vertex.z < avatar.boundingBox[0].z + z / 2.0) {
					apparel.particlesF[i].vertex.z = avatar.boundingBox[0].z + z / 2.0;
				}

				//if (apparel.particlesF[i].isEdge) {
					apparel.particlesF[i].isStatic = true;
				//}

				found = true;
				break;
			}
		}

		if (found == false) {
			apparel.particlesF[i].vertex.z = avatar.boundingBox[0].z + z / 2.0;
		}

	}
}

void ModelGenerator::intersectBackWithAvatar() {
	
	RayCaster rayCaster;
	float z = glm::abs(avatar.boundingBox[1].z - avatar.boundingBox[0].z);

	for (int i = 0; i < apparel.particlesB.size(); i++) {

		glm::vec3 intersectionPoint;
		float distance;
		Ray ray;
		ray.orig = apparel.particlesB[i].vertex;
		ray.dir = glm::vec3(0, 0, -1.0);

		bool found = false;

		for (int j = 0; j < avatar.triangles.size(); j++) {

			if (rayCaster.isRayTriangleIntersect(avatar.triangles[j], ray, &intersectionPoint, &distance)) {

				apparel.particlesB[i].vertex = intersectionPoint;

				if (apparel.particlesB[i].vertex.z > avatar.boundingBox[0].z + z / 2.0) {
					apparel.particlesB[i].vertex.z = avatar.boundingBox[0].z + z / 2.0;
				}

				//if (apparel.particlesB[i].isEdge) {
					apparel.particlesB[i].isStatic = true;
				//}

				found = true;
				break;
			}
		}

		if (found == false) {
			apparel.particlesB[i].vertex.z = avatar.boundingBox[0].z + z / 2.0;
		}

	}

}

void ModelGenerator::intersectApparelWithAvatar() {

	intersectFrontWithAvatar();
	intersectBackWithAvatar();


}

void ModelGenerator::stitchFrontAndBack() {

	for (int i = 0; i < apparel.stitchingPointsF.size(); i++) {
		
		int j = 0;
		
		while (j < apparel.stitchingPointsF[i].size()-1 && j < apparel.stitchingPointsB[i].size()-1) {

			Triangle t1 = Triangle(apparel.stitchingPointsF[i][j], apparel.stitchingPointsB[i][j], apparel.stitchingPointsF[i][j + 1]);
			Triangle t2 = Triangle(apparel.stitchingPointsF[i][j + 1], apparel.stitchingPointsB[i][j], apparel.stitchingPointsB[i][j + 1]);

			apparel.trianglesB.push_back(t1);
			apparel.trianglesB.push_back(t2);

			glm::vec3 normal = glm::cross(t1.p2->vertex - t1.p1->vertex, t1.p3->vertex - t1.p1->vertex);
			
			t1.p1->normal += normal;
			t1.p2->normal += normal;
			t1.p3->normal += normal;

			normal = glm::cross(t2.p2->vertex - t2.p1->vertex, t2.p3->vertex - t2.p1->vertex);
			t2.p1->normal += normal;
			t2.p2->normal += normal;
			t2.p3->normal += normal;

			j++;
		}

		while (j < apparel.stitchingPointsF[i].size()-1) {
			
			Triangle t1 = Triangle(apparel.stitchingPointsF[i][j], apparel.stitchingPointsB[i][apparel.stitchingPointsB[i].size()-1], apparel.stitchingPointsF[i][j + 1]);	
			apparel.trianglesB.push_back(t1);
			
			glm::vec3 normal = glm::cross(t1.p2->vertex - t1.p1->vertex, t1.p3->vertex - t1.p1->vertex);

			t1.p1->normal += normal;
			t1.p2->normal += normal;
			t1.p3->normal += normal;

			j++;
		}

		while (j < apparel.stitchingPointsB[i].size() - 1) {

			Triangle t1 = Triangle(apparel.stitchingPointsB[i][j], apparel.stitchingPointsF[i][apparel.stitchingPointsF[i].size() - 1], apparel.stitchingPointsB[i][j + 1]);
			glm::vec3 normal = glm::cross(t1.p2->vertex - t1.p1->vertex, t1.p3->vertex - t1.p1->vertex);

			t1.p1->normal += normal;
			t1.p2->normal += normal;
			t1.p3->normal += normal;
			
			apparel.trianglesB.push_back(t1);
			
			j++;
		}

	}

}


void ModelGenerator::storeModel(const char* frontImageFileName, const char* backImageFileName) {

	CreateDirectory("C:\\Users\\DELL\\source\\repos\\RapidDigitizationOfApparels\\RapidDigitizationOfApparels\\Model", NULL);
	CopyFile(frontImageFileName, "C:\\Users\\DELL\\source\\repos\\RapidDigitizationOfApparels\\RapidDigitizationOfApparels\\Model\\front.jpg", false);
	CopyFile(backImageFileName, "C:\\Users\\DELL\\source\\repos\\RapidDigitizationOfApparels\\RapidDigitizationOfApparels\\Model\\back.jpg", false);


	FILE* fp = fopen("C:\\Users\\DELL\\source\\repos\\RapidDigitizationOfApparels\\RapidDigitizationOfApparels\\Model\\apparel.obj", "w");

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
		//fprintf(fp, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", index1, index1, index1, index2, index2, index2, index3, index3, index3);
	}

	fclose(fp);

	FILE* fp2 = fopen("C:\\Users\\DELL\\source\\repos\\RapidDigitizationOfApparels\\RapidDigitizationOfApparels\\Model\\apparel.mtl", "w");

	fprintf(fp2, "newmtl front\n");
	fprintf(fp2, "map_Ka front.jpg \n");

	fprintf(fp2, "newmtl back\n");
	fprintf(fp2, "map_Ka back.jpg \n");
}
