#pragma once

#include<glm/glm.hpp>

class Component
{
public:
	Component();
	~Component();

	virtual void update() = 0;
	virtual void render() = 0;

protected:
	unsigned short type;

private:

};


class FixedPoint : public Component
{
public:

	glm::vec3 position;

	FixedPoint();
	~FixedPoint();
	virtual void update();
	virtual void render();

private:

};


class Spring : public Component
{
	// no mass no collision
	// generating force depends on dL
public:
	Spring();
	~Spring();
	virtual void update();
	virtual void render();

private:
	// unsigned short type;
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
	Ball();
	~Ball();

	virtual void update();
	virtual void render();
private:
	// unsigned short type;
	glm::vec3 pos;
	float radius;
	float density;

};