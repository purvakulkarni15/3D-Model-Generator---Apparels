#include "Apparel.h"



Apparel::Apparel()
{
}


Apparel::~Apparel()
{
}


void Apparel::setTrianglesF(cv::Subdiv2D subdiv, Particle** matrix) {

	std::vector<cv::Vec6f> triangleList;
	std::vector<cv::Vec4f> edgeList;
	std::vector<cv::Point> contour;
	subdiv.getTriangleList(triangleList);
	subdiv.getEdgeList(edgeList);


	for (int i = 0; i < boundaryPointsF.size(); i++) {
		contour.push_back(cv::Point(boundaryPointsF[i]->vertex.x, boundaryPointsF[i]->vertex.y));
	}


	for (int i = 0; i < edgeList.size(); i++) {
		cv::Vec4f t = edgeList[i];

		int w1 = (int)t[0];
		int w2 = (int)t[2];

		int h1 = (int)t[1];
		int h2 = (int)t[3];

		if ((h1 < bbHeightF && h1 > 0) && (h2 < bbHeightF && h2 > 0) && (w1 < bbWidthF && w1 > 0) && (w2 < bbWidthF && w2 > 0)) {

			Particle* p1 = matrix[h1 * bbWidthF + w1];
			Particle* p2 = matrix[h2 * bbWidthF + w2];

			Edge e = Edge(p1, p2);
			e.remove = false;
			edgesF.push_back(e);
		}

	}

	for (int i = 0; i < triangleList.size(); i++)
	{

		cv::Vec6f t = triangleList[i];

		int w1 = (int)t[0];
		int w2 = (int)t[2];
		int w3 = (int)t[4];

		int h1 = (int)t[1];
		int h2 = (int)t[3];
		int h3 = (int)t[5];

		Particle* p3 = matrix[h1 * bbWidthF + w1];
		Particle* p2 = matrix[h2 * bbWidthF + w2];
		Particle* p1 = matrix[h3 * bbWidthF + w3];


		glm::vec2 midPt1, midPt2, midPt3;
		midPt1 = glm::vec2((p1->vertex.x + p2->vertex.x)*0.5, (p1->vertex.y + p2->vertex.y)*0.5);
		midPt2 = glm::vec2((p1->vertex.x + p3->vertex.x)*0.5, (p1->vertex.y + p3->vertex.y)*0.5);
		midPt3 = glm::vec2((p3->vertex.x + p2->vertex.x)*0.5, (p3->vertex.y + p2->vertex.y)*0.5);

		if (cv::pointPolygonTest(contour, cv::Point2f(midPt1.x, midPt1.y), false) >= 0 &&
			cv::pointPolygonTest(contour, cv::Point2f(midPt2.x, midPt2.y), false) >= 0 &&
			cv::pointPolygonTest(contour, cv::Point2f(midPt3.x, midPt3.y), false) >= 0)
		{
			Triangle t = Triangle(p1, p2, p3);
			glm::vec3 normal = glm::cross(t.p2->vertex - t.p1->vertex, t.p3->vertex - t.p1->vertex);
			p1->normal += normal;
			p2->normal += normal;
			p3->normal += normal;
			trianglesF.push_back(t);
		}
		else {
			for (int i = 0; i < edgesF.size(); i++) {
				if ((edgesF[i].p1 == p1 && edgesF[i].p2 == p2) ||
					(edgesF[i].p1 == p2 && edgesF[i].p2 == p1)) {
					edgesF[i].remove = true;
				}
				if ((edgesF[i].p1 == p1 && edgesF[i].p2 == p3) ||
					(edgesF[i].p1 == p3 && edgesF[i].p2 == p1)) {
					edgesF[i].remove = true;
				}
				if ((edgesF[i].p1 == p2 && edgesF[i].p2 == p3) ||
					(edgesF[i].p1 == p3 && edgesF[i].p2 == p2)) {
					edgesF[i].remove = true;
				}
			}
		}

	}
}

void Apparel::setTrianglesB(cv::Subdiv2D subdiv, Particle** matrix) {

	std::vector<cv::Vec6f> triangleList;
	std::vector<cv::Vec4f> edgeList;
	std::vector<cv::Point> contour;
	subdiv.getTriangleList(triangleList);
	subdiv.getEdgeList(edgeList);

	for (int i = 0; i < boundaryPointsB.size(); i++) {
		contour.push_back(cv::Point(boundaryPointsB[i]->vertex.x, boundaryPointsB[i]->vertex.y));
	}

	for (int i = 0; i < edgeList.size(); i++) {
		cv::Vec4f t = edgeList[i];

		int w1 = (int)t[0];
		int w2 = (int)t[2];

		int h1 = (int)t[1];
		int h2 = (int)t[3];

		if ((h1 < bbHeightB && h1 > 0) && (h2 < bbHeightB && h2 > 0) && (w1 < bbWidthB && w1 > 0) && (w2 < bbWidthB && w2 > 0)) {

			Particle* p1 = matrix[h1 * bbWidthB + w1];
			Particle* p2 = matrix[h2 * bbWidthB + w2];

			Edge e = Edge(p1, p2);
			e.remove = false;
			edgesB.push_back(e);
		}

	}

	for (int i = 0; i < triangleList.size(); i++)
	{

		cv::Vec6f t = triangleList[i];

		int w1 = (int)t[0];
		int w2 = (int)t[2];
		int w3 = (int)t[4];

		int h1 = (int)t[1];
		int h2 = (int)t[3];
		int h3 = (int)t[5];

		Particle* p1 = matrix[h1 * bbWidthB + w1];
		Particle* p2 = matrix[h2 * bbWidthB + w2];
		Particle* p3 = matrix[h3 * bbWidthB + w3];

		glm::vec2 midPt1, midPt2, midPt3;
		midPt1 = glm::vec2((p1->vertex.x + p2->vertex.x)*0.5, (p1->vertex.y + p2->vertex.y)*0.5);
		midPt2 = glm::vec2((p1->vertex.x + p3->vertex.x)*0.5, (p1->vertex.y + p3->vertex.y)*0.5);
		midPt3 = glm::vec2((p3->vertex.x + p2->vertex.x)*0.5, (p3->vertex.y + p2->vertex.y)*0.5);

		if (cv::pointPolygonTest(contour, cv::Point2f(midPt1.x, midPt1.y), false) >= 0 &&
			cv::pointPolygonTest(contour, cv::Point2f(midPt2.x, midPt2.y), false) >= 0 &&
			cv::pointPolygonTest(contour, cv::Point2f(midPt3.x, midPt3.y), false) >= 0)
		{
			Triangle t = Triangle(p1, p2, p3);
			glm::vec3 normal = glm::cross(t.p2->vertex - t.p1->vertex, t.p3->vertex - t.p1->vertex);
			p1->normal += normal;
			p2->normal += normal;
			p3->normal += normal;
			trianglesB.push_back(t);
		}
		else {
			for (int i = 0; i < edgesB.size(); i++) {
				if ((edgesB[i].p1 == p1 && edgesB[i].p2 == p2) ||
					(edgesB[i].p1 == p2 && edgesB[i].p2 == p1)) {
					edgesB[i].remove = true;
				}
				if ((edgesB[i].p1 == p1 && edgesB[i].p2 == p3) ||
					(edgesB[i].p1 == p3 && edgesB[i].p2 == p1)) {
					edgesB[i].remove = true;
				}
				if ((edgesB[i].p1 == p2 && edgesB[i].p2 == p3) ||
					(edgesB[i].p1 == p3 && edgesB[i].p2 == p2)) {
					edgesB[i].remove = true;
				}
			}
		}
	}
}

void Apparel::modifyObject(glm::vec3 scale, glm::vec3 translate) {

	for (int i = 0; i < particlesF.size(); i++) {
		particlesF[i].vertex.x = particlesF[i].vertex.x * scale.x + translate.x;
		particlesF[i].vertex.y = particlesF[i].vertex.y * scale.y + translate.y;
	}

	for (int i = 0; i < particlesB.size(); i++) {
		particlesB[i].vertex.x = particlesB[i].vertex.x * scale.x + translate.x;
		particlesB[i].vertex.y = particlesB[i].vertex.y * scale.y + translate.y;
	}

}

