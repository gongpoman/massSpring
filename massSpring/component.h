#pragma once

#include<glm/glm.hpp>

#include"shader.h"

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

	unsigned int VAO,EBO;

private:

};


class FixedPoint : public Component
{
public:

	glm::vec3 pos;
	//Shader* shaderID;

	FixedPoint();
	virtual ~FixedPoint();
	virtual void update();
	virtual void render();

private:

	// unsigned int VAO,EBO;
	//unsigned short type;

	virtual void __drawSetup();
};


class Spring : public Component
{
	// no mass no collision
	// generating force depends on dL
public:

	//Shader* shaderID;

	Spring();
	virtual ~Spring();
	virtual void update();
	virtual void render();

private:
	// unsigned short type;
	//unsigned int VAO,EBO;
	float elasticity;
	float defaultLen;
	float currentLen;
	glm::vec3 pos1;
	glm::vec3 pos2;
	
	Component* connectedObj1;
	Component* connectedObj2;

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
	//unsigned int VAO,EBO;
	glm::vec3 pos;
	float radius;
	float density;

	virtual void __drawSetup();
};