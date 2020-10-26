#include "ClothSimulator.h"


ClothSimulator::ClothSimulator() {

}



ClothSimulator::~ClothSimulator()
{

}

void ClothSimulator::addConstraints(Apparel* apparel)
{
	/*
		Add constraints for front part
	*/

	int sizeX = apparel->bbWidthF;
	int sizeY = apparel->bbHeightF;

	for (int i = 0; i < apparel->edgesF.size(); i++) {
		//Add Structural Constraints
		if (!apparel->edgesF[i].remove) {
			constraintsF.push_back(Constraint(apparel->edgesF[i].p1, apparel->edgesF[i].p2));
		}

	}

	for (int i = 0; i < apparel->boundaryPointsF.size()-2; i++) {
		
		//Add Flexion Constraints
		constraintsF.push_back(Constraint(apparel->boundaryPointsF[i], apparel->boundaryPointsF[i+2]));
	}

	constraintsF.push_back(Constraint(apparel->boundaryPointsF[apparel->boundaryPointsF.size() - 2], apparel->boundaryPointsF[0]));
	constraintsF.push_back(Constraint(apparel->boundaryPointsF[apparel->boundaryPointsF.size() - 1], apparel->boundaryPointsF[1]));

	for (int i = 0; i < sizeY; i+= apparel->interval*2)
	{
		for (int j = 0; j < sizeX; j+= apparel->interval*2)
		{
			//Add Bend(Flexion) Constraints
			if (j + apparel->interval * 4 < sizeX) {
				if (clothMatrixF[i*sizeX + j]->index != -1 && clothMatrixF[i*sizeX + j + apparel->interval * 4]->index != -1) {
					constraintsF.push_back(Constraint(clothMatrixF[i*sizeX + j], clothMatrixF[i*sizeX + j + apparel->interval * 4]));
				}
			}
			if (i + apparel->interval * 4 < sizeY) {
				if (clothMatrixF[i*sizeX + j]->index != -1 && clothMatrixF[(i + apparel->interval * 4) * sizeX + j]->index != -1) {
					constraintsF.push_back(Constraint(clothMatrixF[i*sizeX + j], clothMatrixF[(i + apparel->interval * 4)*sizeX + j]));
				}
			}
		}
	}

	/*
		Add constraints for back part
	*/

	sizeX = apparel->bbWidthB;
	sizeY = apparel->bbHeightB;

	for (int i = 0; i < apparel->edgesB.size(); i++) {
		//Add Structural Constraints
		if (!apparel->edgesB[i].remove) {
			constraintsB.push_back(Constraint(apparel->edgesB[i].p1, apparel->edgesB[i].p2));
		}
	}

	for (int i = 0; i < apparel->boundaryPointsB.size() - 2; i++) {
		//Add Flexion Constraints
		constraintsB.push_back(Constraint(apparel->boundaryPointsB[i], apparel->boundaryPointsB[i + 2]));
	}

	constraintsB.push_back(Constraint(apparel->boundaryPointsB[apparel->boundaryPointsB.size() - 2], apparel->boundaryPointsB[0]));
	constraintsB.push_back(Constraint(apparel->boundaryPointsB[apparel->boundaryPointsB.size() - 1], apparel->boundaryPointsB[1]));

	for (int i = 0; i < sizeY; i += apparel->interval * 2)
	{
		for (int j = 0; j < sizeX; j += apparel->interval * 2)
		{
			//Add Bend(Flexion) Constraints
			if (j + apparel->interval * 4 < sizeX) {
				if (clothMatrixB[i*sizeX + j]->index != -1 && clothMatrixB[i*sizeX + j + apparel->interval * 4]->index != -1) {
					constraintsB.push_back(Constraint(clothMatrixB[i*sizeX + j], clothMatrixB[i*sizeX + j + apparel->interval * 4]));
				}
			}
			if (i + apparel->interval * 4 < sizeY) {
				if (clothMatrixB[i*sizeX + j]->index != -1 && clothMatrixB[(i + apparel->interval * 4) * sizeX + j]->index != -1) {
					constraintsB.push_back(Constraint(clothMatrixB[i*sizeX + j], clothMatrixB[(i + apparel->interval * 4)*sizeX + j]));
				}
			}
		}
	}
}

void ClothSimulator::setClothMatrixF(Apparel* apparel) {

	clothMatrixF = (Particle**)malloc(sizeof(Particle*)*apparel->bbWidthF*apparel->bbHeightF);

	for (int i = 0; i < apparel->bbHeightF; i++)
	{
		for (int j = 0; j < apparel->bbWidthF; j++)
		{
			Particle* p = (Particle*)malloc(sizeof(Particle));
			p->index = -1;
			clothMatrixF[i*apparel->bbWidthF + j] = p;
		}
	}

	for (int i = 0; i < apparel->particlesF.size(); i++)
	{
		int col = apparel->particlesF[i].vertex.x;
		int row = apparel->particlesF[i].vertex.y;

		clothMatrixF[row*apparel->bbWidthF + col] = &apparel->particlesF[i];
		clothMatrixF[row*apparel->bbWidthF + col]->index = i+1;
	}
}

void ClothSimulator::setClothMatrixB(Apparel* apparel) {

	clothMatrixB = (Particle**)malloc(sizeof(Particle*)*apparel->bbWidthB*apparel->bbHeightB);

	for (int i = 0; i < apparel->bbHeightB; i++)
	{
		for (int j = 0; j < apparel->bbWidthB; j++)
		{
			Particle* p = (Particle*)malloc(sizeof(Particle));
			p->index = -1;
			clothMatrixB[i*apparel->bbWidthB + j] = p;
		}
	}

	for (int i = 0; i < apparel->particlesB.size(); i++)
	{
		int col = apparel->particlesB[i].vertex.x;
		int row = apparel->particlesB[i].vertex.y;

		clothMatrixB[row*apparel->bbWidthB + col] = &apparel->particlesB[i];
		clothMatrixB[row*apparel->bbWidthB + col]->index = i + apparel->particlesF.size() + 1;
	}
}

void ClothSimulator::satisfyConstraints(int i){
	
	if (i == 1) {

		for (int i = 0; i < 50; i++)
		{
			for (int j = 0; j < constraintsF.size(); j++)
			{
				constraintsF[j].satisfy(i);
			}
		}


		for (int i = 0; i < 50; i++)
		{
			for (int j = 0; j < constraintsB.size(); j++)
			{
				constraintsB[j].satisfy(i);
			}
		}
	}else if (i == 2) {

		for (int i = 0; i < 1; i++)
		{
			for (int j = 0; j < constraintsF.size(); j++)
			{
				constraintsF[j].satisfy(i);
			}
		}


		for (int i = 0; i < 1; i++)
		{
			for (int j = 0; j < constraintsB.size(); j++)
			{
				constraintsB[j].satisfy(i);
			}
		}
	}

}
