#include<iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "msSystem.h"
#include "component.h"


msSystem::msSystem(glm::vec3 gPos, float dt) {
	globalPos = gPos;
	deltaTime = dt;
}
msSystem::~msSystem() {
	Component* delC;

	for (auto iter = components.begin(); iter != components.end(); iter++) {
		delC = *(iter);
		delete delC;
	}
}

void msSystem::addComponent(Component* thing) {
	components.push_back(thing);
	std::cout << "type number " << thing->getType()<< " added in compList" << std::endl;
}

void msSystem::updateAll() {
	std::cout << "update start!!=========================================" << std::endl;

	// in update i want collectively update pos, vel, acc after F is defined.
	// because elastic force is depends on position
	// so before update, F must be defined.
	for (auto iter = components.begin(); iter != components.end(); iter++) {
		CompType t = (*iter)->getType();
		if (t == MASS)
			static_cast<Mass*>(*iter)->ftProcess();
	}

	for (auto iter = components.begin(); iter != components.end(); iter++) {
		if(( * iter)->getType() != SPRINGL)
			(*iter)->update();
	}
	for (auto iter = components.begin(); iter != components.end(); iter++) {
		if ((*iter)->getType() == SPRINGL)
			(*iter)->update();
	}

	// TODO collision detection

	// TODO collision resolution


	std::cout << "update end!!=========================================" << std::endl;
}
void msSystem::renderAll() {
	std::cout << "render start !!! ==============================================" << std::endl;
	// render components
	for (auto iter = components.begin();iter!=components.end();iter++) {
		(*iter)->render(globalPos);
		
	}
	std::cout << "render end!! =============================================== " << std::endl;
}

