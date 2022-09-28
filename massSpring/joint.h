#pragma once

#include<vector> 

#include<glm/glm.hpp>

#include"component.h"

//BUG 이거 왜 써야 되나. 이게 compoenent.h에 정의되어 있는데...
class Mass;
class Component;
class SpringL;
class FixedPoint;


// interface btw mass and spring.
class Joint
{
public:
	Joint();
	Joint(FixedPoint* fp);
	Joint(Mass* mass, glm::vec3 jpos);
	~Joint();

	void addSpring(SpringL* pSpr,bool id);

	//function return netforce which springs exert.
	glm::vec3 getJointForce();
	//TODO function return pos where spring linked
	glm::vec3 getJointPos();

private:

	glm::vec3 jPos; // displacement from pos of mass => joint pos = mass->pos + jPos
	Component* compo; // mass or fixedpoint

	std::vector<SpringL*> springs; // linked springs
	std::vector<bool> endId;// endPoint of spring ID : false -> point 1, true -> point 2,


	


};
