#pragma once
#include "Particle.h"
class Triangle
{
public:

	Particle *p1, *p2, *p3;
	Triangle();
	Triangle(Particle*, Particle*, Particle*);
	~Triangle();
};

