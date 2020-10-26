#pragma once
#include "Particle.h"
class Constraint
{
public:
	Particle *p1, *p2;
	float distance;
	float k, b;

	Constraint(Particle*, Particle*);
	void satisfy(int i);

};

