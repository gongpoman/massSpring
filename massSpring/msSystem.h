#pragma once

#include<vector>

#include"component.h"

/* 
* msSysyem : managing spring, mass...
* let every obj call update, render...
*/

struct { // TODO collision
}collision;

class msSystem
{
public: // REF :  most vexing https://stackoverflow.com/questions/20871225/error-c2228-left-of-must-have-class-struct-union
	
	std::vector<Component*> components;

	glm::vec3 globalPos;

	float deltaTime;
	
	msSystem(glm::vec3 gPos = glm::vec3(0), float dt = 60.0f);
	~msSystem();

	void addComponent(Component*);

	void updateAll();
	void renderAll();


private:

};

