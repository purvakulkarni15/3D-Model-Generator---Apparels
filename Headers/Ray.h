#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Ray
{
public:

	glm::vec3 dir;
	glm::vec3 orig;

	Ray();
	~Ray();
};

