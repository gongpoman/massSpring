#include<iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>

#include"component.h"
#include"msSystem.h"
#include"shader.h"

Component::Component()
{
}

Component::~Component()
{
}


FixedPoint::FixedPoint()
{
}

FixedPoint::~FixedPoint()
{
}


Spring::Spring()
{
}

Spring::~Spring()
{
}


Ball::Ball()
{
}

Ball::~Ball()
{
}


void FixedPoint::render() {

}

void FixedPoint::update() {

}
void FixedPoint::drawSetup() {
	shaderID = new Shader("resources/shader/box_vs.txt", "resources/shader/box_fs.txt");

}


void Spring::render() {

}

void Spring::update() {

}
void Spring::drawSetup() {

}

void Ball::drawSetup() {

}

void Ball::render() {

}

void Ball::update() {

}