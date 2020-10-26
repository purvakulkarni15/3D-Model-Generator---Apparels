#pragma once
#include "Triangle.h"
#include <vector>

using namespace std;

class Avatar
{
public:

	vector<Particle> particles;
	vector<Triangle> triangles;
	glm::vec3 boundingBox[2];

	Avatar();
	~Avatar();
	void loadModel(const char* file);
};

