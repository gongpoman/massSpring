#pragma once

#include<vector>

#include"component.h"

/* 
* msSysyem : managing spring, mass...
* let every obj call update, render...
*/

struct { // TODO later...
}collision;

class msSystem
{
public: // TODO ¸ô¶ú´ø °Å. most vexing https://stackoverflow.com/questions/20871225/error-c2228-left-of-must-have-class-struct-union

	std::vector<Component*> components;

	msSystem();
	~msSystem();

	void updateAll();
	void renderAll();

private:

};

