#pragma once
#include "Particle.h"
class Constraint
{
public:
	Particle *p1, *p2;
	float distance;
	float k, b;
	int constraintType;

	Constraint(Particle*, Particle*, int);
	void satisfy(bool, bool, bool, int);

};

