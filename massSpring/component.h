#pragma once
#include<vector>

#include<glm/glm.hpp>

#include"shader.h"


/* 
using SI unit
length : m, mass : kg, ...
*/

class Joint;

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

	void addJoint(Joint*);

private:

	std::vector<Joint*> joints;

	// unsigned int VAO;
	//CompType type;

	virtual void __drawSetup();
};


class SpringL : public Component
{
	// no mass no collision
	// generating force depends on dL
	// each end point depends on vertex of linked object.
	// so both points should be linked to move
public:

	//Shader* shaderID;

	SpringL();
	SpringL(glm::vec3 position1 = glm::vec3(0), glm::vec3 position2 = glm::vec3(0), float ela = 1.0f);
	virtual ~SpringL();
	virtual void update();
	virtual void render();

	void linkJoint(Joint*,bool);

private:
	//CompType type;
	//unsigned int VAO;
	unsigned int VBO;

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

	void ftProcess();

	glm::vec3 getPosition();
	void addJoint(Joint*);

protected:
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 acc;
	void* orientation; // TODO how can i represent orientation. use quaternion?

	glm::vec3 netF; // netForce.
	glm::vec3 netT; // netTorque.

	float mass;
	float density;

	std::vector<Joint*> joints;

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


	// float density;
	// float mass;

	float radius;

private:

	virtual void __drawSetup();
};
