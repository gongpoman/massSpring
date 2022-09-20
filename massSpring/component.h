#pragma once

#include<glm/glm.hpp>

#include"shader.h"

class Component
{
public:
	Component();
	~Component();

	Shader* shaderID;

	virtual void drawSetup()=0;
	virtual void update() = 0;
	virtual void render() = 0;

protected:
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
	~FixedPoint();
	virtual void drawSetup();
	virtual void update();
	virtual void render();

private:

	// unsigned int VAO;
};


class Spring : public Component
{
	// no mass no collision
	// generating force depends on dL
public:

	//Shader* shaderID;

	Spring();
	~Spring();
	virtual void drawSetup();
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
	
	Component* connectedObj1;
	Component* connectedObj2;
};

class Ball : public Component	// only ball collide. DCD
{
public:

	//Shader* shaderID;

	Ball();
	~Ball();

	virtual void drawSetup();
	virtual void update();
	virtual void render();
private:
	// unsigned short type;
	//unsigned int VAO;
	glm::vec3 pos;
	float radius;
	float density;
};