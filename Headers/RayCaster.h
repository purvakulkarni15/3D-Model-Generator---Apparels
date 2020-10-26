#pragma once
#include "Ray.h"
#include "Triangle.h"
class RayCaster
{
public:
	bool isRayTriangleIntersect(Triangle t, Ray ray, glm::vec3* intersectionPoint, float* distance);
	RayCaster();
	~RayCaster();
};

