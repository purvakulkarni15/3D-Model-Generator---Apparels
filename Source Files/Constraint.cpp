#include "Constraint.h"

Constraint::Constraint(Particle *p1, Particle *p2, int constraintType)
{
	this->p1 = p1;
	this->p2 = p2;
	this->k = 10.0;
	glm::vec3 vec = p1->vertex - p2->vertex;
	distance = glm::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
	this->constraintType = constraintType;

}

void Constraint::satisfy(bool isFixStitchedPts, bool isFixStaticPts, bool isFixIntersectPts, int constraintType) {

	if (constraintType == 2) {
		if (this->constraintType == 2) {
			glm::vec3 v = p1->vertex - p2->vertex;
			float dist = glm::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
			if (dist > distance) {
				
				v *= ((1.0f - distance / dist) * 0.5f);
				p1->move(-v, isFixStitchedPts, isFixStaticPts, isFixIntersectPts);
				p2->move(v, isFixStitchedPts, isFixStaticPts, isFixIntersectPts);
			}
			return;
		}
		return;
	}

	if (constraintType == 1) {
		if (this->constraintType == 1) {
			glm::vec3 v = p1->vertex - p2->vertex;
			float dist = glm::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
			if (dist > distance) {
				
				v *= ((1.0f - distance / dist) * 0.5f);
				p1->move(-v, isFixStitchedPts, isFixStaticPts, isFixIntersectPts);
				p2->move(v, isFixStitchedPts, isFixStaticPts, isFixIntersectPts);
			}
			return;
		}
		return;
	}
	

	glm::vec3 v = p1->vertex - p2->vertex;
	float dist = glm::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	
	if (dist > distance) {
		v *= ((1.0f - distance / dist) * 0.5f);
		p1->move(-v, isFixStitchedPts, isFixStaticPts, isFixIntersectPts);
		p2->move(v, isFixStitchedPts, isFixStaticPts, isFixIntersectPts);
	}
}