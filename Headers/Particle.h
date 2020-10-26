#pragma once
#ifdef _WIN32
#include <windows.h> 
#endif
#include<time.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Particle
{
public:

	glm::vec3 vertex;
	//glm::vec3 normal;
	glm::vec2 texture;
	/*glm::vec3 force;
	glm::vec3 velocity;
	float damping;
	float mass;*/
	int index;
	int scanLine;
	bool isEdge;
	bool isStatic;
	bool isIntersect;
	bool isStitched;

	Particle();
	Particle(glm::vec3 vertex);
	void move(glm::vec3, bool, bool, bool);
	~Particle();
};

