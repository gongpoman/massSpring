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
}
Joint::Joint(Mass* mass, glm::vec3 jpos) {
	compo = mass;
	jPos = jpos;
}
Joint::~Joint()
{

}

void Joint::addSpring(SpringL* pSpr,bool id) {
	springs.push_back(pSpr);
	endId.push_back(id);
}