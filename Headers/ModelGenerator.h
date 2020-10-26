#pragma once
#include "ImageProcessor.h"
#include "ClothSimulator.h"
class ModelGenerator
{
public:

	Apparel apparel;
	Avatar avatar;
	ClothSimulator clothSimulator;

	ModelGenerator();
	~ModelGenerator();
	void generateModel(ImageProcessor imageProcessorObj);
	void intersectApparelWithAvatar();
	void storeModel(const char*, const char*);
	void setStitchParticles(vector<Particle*>&, vector<vector<Particle*>>&, int, int);
	int getParticleIndex(vector<Particle*> &particles, glm::vec2 point);
	void stitchFrontAndBack();

private:

	void generateModelF(ImageProcessor);
	void generateModelB(ImageProcessor);
	void intersectFrontWithAvatar();
	void intersectBackWithAvatar();
	glm::vec2* findBoundingBox(vector<vector<Point>>);
	void addPoints(vector<Particle*> &boundaryPoints, vector<Particle>&, vector<vector<Point>>, glm::vec2*, bool);
	Subdiv2D triangulate(vector<Particle>, int, int);
};


