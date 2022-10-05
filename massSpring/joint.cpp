#include<vector>

#include<glm/glm.hpp>

#include"component.h"
#include"joint.h"


Joint::Joint()
{
}

Joint::Joint(FixedPoint* fp) {
	compo = fp;
	jPos = glm::vec3(0);
	fp->addJoint(this);
}
Joint::Joint(Mass* mass, glm::vec3 jpos) {
	compo = mass;
	jPos = jpos;
	mass->addJoint(this);
}
Joint::~Joint()
{

}

void Joint::linkSpring(SpringL* pSpr,bool id) {
	springs.push_back(pSpr);
	endId.push_back(id);
	pSpr->linkJoint(this,id);
}

glm::vec3 Joint::getJointForce() {
	glm::vec3 netForce = glm::vec3(0);

	glm::vec3 force;
	for (int i = 0; i < springs.size();i++) {
		force = springs[i]->getSpringForce(endId[i]);
		netForce += force;
	}

	return netForce;
}
glm::vec3 Joint::getJointPos() {
	glm::vec3 compoCenter;
	if (compo->getType() == FP)
		compoCenter = static_cast<FixedPoint*>(compo)->getPosition();
	else if (compo->getType() == MASS)
		compoCenter = static_cast<Mass*>(compo)->getPosition();
	return compoCenter + jPos;
}