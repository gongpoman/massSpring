#pragma once

#include"object.h"

/* 
* msSysyem : managing spring, mass...
* let every obj call update, render...
*/

struct {
	Object* 
}collision;

class msSystem
{
public:
	msSystem();
	~msSystem();
	void updateAll();

private:

};

msSystem::msSystem()
{
}

msSystem::~msSystem()
{
}
