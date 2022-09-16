#pragma once

#include<vector>

#include<glm/glm.hpp>

class Object
{
public:
	Object();
	~Object();

	virtual void update() = 0;
	virtual void render() = 0;

protected:
	unsigned short type;

private:

};

Object::Object()
{
}

Object::~Object()
{
}


class FixedPoint : public Object
{
public:

	glm::vec3 position;

	FixedPoint();
	~FixedPoint();
	virtual void update();
	virtual void render();

private:

};

FixedPoint::FixedPoint()
{
}

FixedPoint::~FixedPoint()
{
}


class Spring : public Object
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
	
	Object* connectedObj1;
	Object* connectedObj2;
};

Spring::Spring()
{
}

Spring::~Spring()
{
}

class ball : public Object	// only ball collide. DCD
{
public:
	ball();
	~ball();

	virtual void update();
	virtual void render();
private:
	// unsigned short type;
	glm::vec3 pos;
	float radius;
	float density;

};

ball::ball()
{
}

ball::~ball()
{
}