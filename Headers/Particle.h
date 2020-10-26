#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Particle
{
public:

	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec2 texture;
	int index;
	bool isEdge;
	bool isStatic;

	Particle();
	Particle(glm::vec3 vertex);
	Particle(glm::vec3 vertex, glm::vec3 normal, glm::vec2 texture);
	void move(glm::vec3 delta);
	~Particle();
};

