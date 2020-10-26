#include "Constraint.h"

Constraint::Constraint(Particle *p1, Particle *p2)
{
	this->p1 = p1;
	this->p2 = p2;
	this->k = 10.0;
	glm::vec3 vec = p1->vertex - p2->vertex;
	distance = glm::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);

}

void Constraint::satisfy(int i) {
	glm::vec3 v = p1->vertex - p2->vertex;
	float dist = glm::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	
	if (dist > distance) {
		v *= ((1.0f - distance / dist) * 0.5f);
		p1->move(-v, i);
		p2->move(v, i);
	}

	/*if (dist < distance) {
		v *= ((1.0f - dist / distance) * 0.5f);
		p1->move(v);
		p2->move(-v);
	}*/
}
