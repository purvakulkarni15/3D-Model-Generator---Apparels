#include "Triangle.h"

Triangle::Triangle()
{
}

Triangle::Triangle(Particle* p1, Particle* p2, Particle* p3)
{
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
}

Triangle::~Triangle()
{
}
