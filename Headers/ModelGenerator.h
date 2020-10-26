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
	void generate2DModel(ImageProcessor imageProcessorObj);
	void generate3DModel(float xScale, float yScale, glm::vec3 center, const char* front, const char* back);
	void setStitchParticles(vector<Particle*>&, vector<vector<Particle*>>&, int, int);
	int getParticleIndex(vector<Particle*> &particles, glm::vec2 point);


private:

	const char* modelDirectory = "C:\\Users\\DELL\\source\\repos\\RapidDigitizationOfApparels\\RapidDigitizationOfApparels\\Model";
	const char* modelTextureFrontImage = "C:\\Users\\DELL\\source\\repos\\RapidDigitizationOfApparels\\RapidDigitizationOfApparels\\Model\\Front.jpg";
	const char* modelTextureBackImage = "C:\\Users\\DELL\\source\\repos\\RapidDigitizationOfApparels\\RapidDigitizationOfApparels\\Model\\Back.obj";
	const char* modelObj = "C:\\Users\\DELL\\source\\repos\\RapidDigitizationOfApparels\\RapidDigitizationOfApparels\\Model\\apparel.obj";
	const char* modelMtl = "C:\\Users\\DELL\\source\\repos\\RapidDigitizationOfApparels\\RapidDigitizationOfApparels\\Model\\apparel.mtl";

	void generateModelF(ImageProcessor);
	void generateModelB(ImageProcessor);
	void intersectFrontWithAvatar();
	void intersectBackWithAvatar();
	void intersectApparelWithAvatar();
	void storeModel(const char*, const char*);
	void stitchFrontAndBack();
	void move(float distance);
	void scale(float scale);
	glm::vec2* findBoundingBox(vector<vector<Point>>);
	void addPoints(vector<Particle*> &boundaryPoints, vector<Particle>&, vector<vector<Point>>, glm::vec2*, bool);
	Subdiv2D triangulate(vector<Particle>, int, int);
};


