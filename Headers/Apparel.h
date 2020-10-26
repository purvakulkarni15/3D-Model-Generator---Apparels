#pragma once
#include "Triangle.h"
#include "Edge.h"
#include "RayCaster.h"
#include "Avatar.h"
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Apparel
{
public:
	int bbWidthF, bbHeightF, bbWidthB, bbHeightB; //width and height of bounding box of 2d apparel;
	glm::vec3 boundingBoxF[2], boundingBoxB[2];
	float fHeight, fWidth;
	float bHeight, bWidth;
	int interval;
	float scale;
	
	vector<Edge> edgesF;
	vector<Edge> edgesB;

	vector<vector<Particle*>> stitchingPointsF;
	vector<vector<Particle*>> stitchingPointsB;
	
	vector<Triangle> trianglesF;
	vector<Particle> particlesF;
	
	vector<Triangle> trianglesB;
	vector<Particle> particlesB;

	vector<Particle*> boundaryPointsF;
	vector<Particle*> boundaryPointsB;

	Apparel();
	~Apparel();
	void modifyObject(glm::vec3 scale, glm::vec3 translate);
	void setTrianglesF(cv::Subdiv2D subdiv, Particle** matrix/*, vector<vector<cv::Point>>*/);
	void setTrianglesB(cv::Subdiv2D subdiv, Particle** matrix/*, vector<vector<cv::Point>>*/);
};

