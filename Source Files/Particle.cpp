#include "Particle.h"

Particle::Particle()
{

}

Particle::Particle(glm::vec3 vertex)
{
	this->vertex = vertex;
	this->texture = glm::vec2(vertex.x, vertex.y);
	//this->normal = glm::vec3(0, 0, 0);
	this->isEdge = false;
	this->isStatic = false;
	this->isStitched = false;
	this->isIntersect = false;
	/*this->mass = 1.0;
	this->damping = 0.4;
	this->force = glm::vec3(0.0f, 0.0f, 0.0f);
	this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);*/
}

Particle::~Particle(){}

void Particle::move(glm::vec3 delta, bool isFixStitchedPts, bool isFixStaticPts, bool isFixIntersectionPts) {
	if (isFixIntersectionPts && isIntersect) return;
	else if (isFixStitchedPts && isStitched) return;
	else if (isFixStaticPts && isStatic) return;
	vertex += delta;
}