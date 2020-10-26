#include "Avatar.h"

Avatar::Avatar()
{
}


Avatar::~Avatar()
{
}


void Avatar::loadModel(const char* file)
{
	FILE* fp;
	fp = fopen(file, "r");
	int vIndex = 0;
	int vnIndex = 0;

	particles.clear();
	triangles.clear();

	if (fp != NULL)
	{
		char ch;

		glm::vec3 p;

		boundingBox[0].x = 9999.0;
		boundingBox[0].y = 9999.0;
		boundingBox[0].z = 9999.0;
		boundingBox[1].x = -9999.0;
		boundingBox[1].y = -9999.0;
		boundingBox[1].z = -9999.0;

		while ((ch = fgetc(fp)) != EOF)
		{
			if (ch == '\n')
				continue;
			if (ch == 'v')
			{
				ch = fgetc(fp);

				fscanf(fp, "%f %f %f\n", &p.x, &p.y, &p.z);

				Particle particle = Particle(glm::vec3(60.0*p.x, 60.0*p.y, 60.0*p.z));

				if (boundingBox[0].x > particle.vertex.x) {
					boundingBox[0].x = particle.vertex.x;
				}
				if (boundingBox[0].y > particle.vertex.y) {
					boundingBox[0].y = particle.vertex.y;
				}
				if (boundingBox[0].z > particle.vertex.z) {
					boundingBox[0].z = particle.vertex.z;
				}
				if (boundingBox[1].x < particle.vertex.x) {
					boundingBox[1].x = particle.vertex.x;
				}
				if (boundingBox[1].y < particle.vertex.y) {
					boundingBox[1].y = particle.vertex.y;
				}
				if (boundingBox[1].z < particle.vertex.z) {
					boundingBox[1].z = particle.vertex.z;
				}

				particle.index = vIndex;
				particles.push_back(particle);
				vIndex++;
			}
			else if (ch == 'f')
			{
				ch = fgetc(fp);

				int vindex = -1;
				int vindexVal = 0;

				int vertices[3];

				do
				{
					if (fscanf(fp, "%d", &vindexVal) != 0)
					{
						vertices[++vindex] = vindexVal;
						ch = fgetc(fp);
					}
					else
					{
						break;
					}
				} while (ch != '\n');

				Triangle t = Triangle(&particles[vertices[0] - 1], &particles[vertices[1] - 1], &particles[vertices[2] - 1]);
				triangles.push_back(t);

			}
			else
			{
				char c[1024];
				fscanf(fp, "%[^\n]\n", c);
			}
		}
	}

	fclose(fp);

	float width, height, depth;
	float x, y, z;
	width = glm::abs(boundingBox[1].x - boundingBox[0].x);
	height= glm::abs(boundingBox[1].y - boundingBox[0].y);
	depth = glm::abs(boundingBox[1].z - boundingBox[0].z);

	x = boundingBox[0].x;
	y = boundingBox[0].y;
	z = boundingBox[0].z;

	boundingBox[0].x = 9999.0;
	boundingBox[0].y = 9999.0;
	boundingBox[0].z = 9999.0;
	boundingBox[1].x = -9999.0;
	boundingBox[1].y = -9999.0;
	boundingBox[1].z = -9999.0;


	for (int i = 0; i < particles.size(); i++) {

		particles[i].vertex.x = ((particles[i].vertex.x - x)- width / 2.0);
		particles[i].vertex.y = ((particles[i].vertex.y - y)- height / 2.0);
		particles[i].vertex.z = ((particles[i].vertex.z - z)- depth / 2.0);

		if (boundingBox[0].x > particles[i].vertex.x) {
			boundingBox[0].x = particles[i].vertex.x;
		}
		if (boundingBox[0].y > particles[i].vertex.y) {
			boundingBox[0].y = particles[i].vertex.y;
		}
		if (boundingBox[0].z > particles[i].vertex.z) {
			boundingBox[0].z = particles[i].vertex.z;
		}
		if (boundingBox[1].x < particles[i].vertex.x) {
			boundingBox[1].x = particles[i].vertex.x;
		}
		if (boundingBox[1].y < particles[i].vertex.y) {
			boundingBox[1].y = particles[i].vertex.y;
		}
		if (boundingBox[1].z < particles[i].vertex.z) {
			boundingBox[1].z = particles[i].vertex.z;
		}



	}
}