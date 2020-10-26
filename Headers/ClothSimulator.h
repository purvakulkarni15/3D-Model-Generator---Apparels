#pragma once
#include "Apparel.h"
#include "Constraint.h"

class ClothSimulator
{
public:

	vector<Constraint> constraintsF;
	vector<Constraint> constraintsB;

	Particle** clothMatrixF;
	Particle** clothMatrixB;

	ClothSimulator();
	~ClothSimulator();

	void setClothMatrixF(Apparel* apparel);
	void setClothMatrixB(Apparel* apparel);

	void addConstraints(Apparel* apparel);
	void satisfyConstraints(int i);
};

