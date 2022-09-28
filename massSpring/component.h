#pragma once
#include<vector>

#include<glm/glm.hpp>

#include"shader.h"

#include"joint.h" 

/* 
using SI unit
length : m, mass : kg, ...
*/

enum CompType {
	SPRINGL,
	MASS,
	FP
};

class Component
{
public:
	Component();
	virtual ~Component();

	Shader* shader;
	virtual void update() = 0;
	virtual void render() = 0;

	CompType getType();

protected:
	CompType type;
	unsigned int VAO;

	virtual void __drawSetup() = 0;
private:

};


class FixedPoint : public Component
{
public:

	glm::vec3 pos;
	//Shader* shaderID;

	FixedPoint();
	FixedPoint(glm::vec3 position);
	virtual ~FixedPoint();
	virtual void update();
	virtual void render();

	glm::vec3 getPosition();

private:

	// unsigned int VAO;
	//CompType type;

	virtual void __drawSetup();
};


class SpringL : public Component
{
	// no mass no collision
	// generating force depends on dL
	// each end point depends on vertex of linked object.
public:

	//Shader* shaderID;

	SpringL();
	SpringL(glm::vec3 position1, glm::vec3 position2, float ela = 1.0f);
	virtual ~SpringL();
	virtual void update();
	virtual void render();

private:
	//CompType type;
	//unsigned int VAO;
	float elasticity;
	float defaultLen;
	float currentLen;
	glm::vec3 pos1;
	glm::vec3 pos2;

	Joint* endP[2];

	virtual void __drawSetup();
};

class Mass : public Component {
public : 
	Mass();
	virtual ~Mass();
	virtual void update() = 0;
	virtual void render() = 0;

	virtual void ftProcess()=0;

	glm::vec3 getPosition();
protected:
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 acc;
	void* orientation; // TODO how can i represent orientation. use quaternion?

	glm::vec3 netF; // netForce.
	glm::vec3 netT; // netTorque.

private : 

	virtual void __drawSetup() = 0;

};


class Ball : public Mass // only ball collide. DCD
{
public:

	//Shader* shaderID;

	Ball();
	Ball(glm::vec3 position,float r = 1.0f, float d = 1.0f);
	virtual ~Ball();


	virtual void update();
	virtual void render();

protected : 

	/*
	* first we calculate these values in ftProcess function and in update func pos, vel, acc updated.
	glm::vec3 netF; // netForce.
	glm::vec3 netT; // netTorque.
	
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 acc;

	*/

	//CompType type;
	//unsigned int VAO;
private:
	
	std::vector<Joint*> joints;

	float radius;
	float density;
	float mass;
	virtual void ftProcess();
	virtual void __drawSetup();
};
