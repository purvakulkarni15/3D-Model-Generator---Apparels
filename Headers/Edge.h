#pragma once
#include "Particle.h"
class Edge
{
public:

	Particle *p1, *p2;
	bool remove;
	Edge(Particle*, Particle*);
	~Edge();
};

