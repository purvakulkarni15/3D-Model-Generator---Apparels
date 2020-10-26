#include<iostream>
#include "RayCaster.h"


RayCaster::RayCaster()
{
}


RayCaster::~RayCaster()
{
}

bool RayCaster::isRayTriangleIntersect(Triangle t, Ray ray, glm::vec3* intersectionPoint, float* distance) {

	double kEpsilon = 0.000001;

	float u, v, w;

	glm::vec3 v0v1 = t.p2->vertex - t.p1->vertex;
	glm::vec3 v0v2 = t.p3->vertex - t.p1->vertex;

	glm::vec3 pvec = glm::cross(ray.dir, v0v2);
	double det = glm::dot(v0v1, pvec);

	if (det < kEpsilon) return false;	

	if (fabs((float)det) < kEpsilon) return false;

	double invDet = 1 / det;

	glm::vec3 tvec = ray.orig - t.p1->vertex;

	u = glm::dot(tvec, pvec) * invDet;

	if (u < 0 || u > 1) return false;

	glm::vec3 qvec = glm::cross(tvec, v0v1);
	
	v = glm::dot(ray.dir, qvec) * invDet;
	
	if (v < 0 || u + v > 1) return false;

	w = glm::dot(v0v2, qvec) * invDet;

	if (w < 0)
	{
		*intersectionPoint = glm::vec3(ray.dir.x*w, ray.dir.y*w, ray.dir.z*w);
		*intersectionPoint = ray.orig + *intersectionPoint;
		glm::vec3 vec = glm::vec3((ray.orig - *intersectionPoint));
		*distance = glm::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
		return true;
	}

	return false;
}
