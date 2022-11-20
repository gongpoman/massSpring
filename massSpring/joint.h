#pragma once

#include<vector> 

#include<glm/glm.hpp>


class Mass;
class Component;
class SpringL;
class FixedPoint;


// interface btw mass and spring.
class Joint
{
public:
	Joint();
	Joint(FixedPoint* fp);					// joint is created on fixed point or Mass
	Joint(Mass* mass, glm::vec3 jpos);
	~Joint();

	void linkSpring(SpringL* pSpr,bool id);	// after joint is created, this func is called to link joint to spring.

	//function return netforce which springs exert.
	glm::vec3 getJointForce();
	//TODO function return pos where spring linked
	glm::vec3 getJointPos();

	glm::vec3 getJointCompVel();

	float getCompMass();
private:

	glm::vec3 jPos; // displacement from pos of mass => joint pos = mass->pos + jPos
	Component* compo; // mass or fixedpoint

	std::vector<SpringL*> springs; // linked springs. SpringL* are deleted by msSystemClass.
	std::vector<bool> endId;// endPoint of spring ID : false -> point 1, true -> point 2,


	


};
