#pragma once

#include<glm/glm.hpp>

#include"shader.h"

/* 
using SI unit
length : m, mass : kg, ...
*/
class Component
{
public:
	Component();
	virtual ~Component();

	Shader* shader;
	virtual void update() = 0;
	virtual void render() = 0;

protected:

	virtual void __drawSetup() = 0;

	unsigned short type;

	unsigned int VAO;

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

private:

	// unsigned int VAO;
	//unsigned short type;

	virtual void __drawSetup();
};


class SpringL : public Component
{
	// no mass no collision
	// generating force depends on dL
public:

	//Shader* shaderID;

	SpringL();
	SpringL(glm::vec3 position1, glm::vec3 position2, float ela=1.0f);
	virtual ~SpringL();
	virtual void update();
	virtual void render();

private:
	// unsigned short type;
	//unsigned int VAO;
	float elasticity;
	float defaultLen;
	float currentLen;
	glm::vec3 pos1;
	glm::vec3 pos2;
	/*
	Component* connectedObj1;
	Component* connectedObj2;

	TODO : think how to do handle interaction btw spring and ball.// I 'm thinking to make obj "joint" which is interface bte them...
	*/
	virtual void __drawSetup();
};

class Ball : public Component	// only ball collide. DCD
{
public:

	//Shader* shaderID;

	Ball();
	Ball(glm::vec3 position,float r = 1.0f, float d = 1.0f);
	virtual ~Ball();

	virtual void update();
	virtual void render();
private:
	// unsigned short type;
	//unsigned int VAO;
	glm::vec3 pos;
	float radius;
	float density;
	float mass;

	virtual void __drawSetup();
};