#include<iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "msSystem.h"
#include "component.h"

msSystem::msSystem() {

}
msSystem::~msSystem() {

}

void msSystem::updateAll() {
	std::cout << "update start!!=========================================" << std::endl;

	// TODO update components loop

	// TODO collision detection

	// TODO collision resolution


	std::cout << "update end!!=========================================" << std::endl;
}
void msSystem::renderAll() {
	std::cout << "render start !!! ==============================================" << std::endl;
	// render components
	for (auto iter = components.begin();iter!=components.end();iter++) {
		(*iter)->render();
		
	}


	std::cout << "render end!! =============================================== " << std::endl;
}

