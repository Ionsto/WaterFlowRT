#pragma once
#include "World.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
class RenderEngine
{
private:
	GLFWwindow* Window;
public:
	Camera cam;
	RenderEngine(GLFWwindow* Window);
	virtual void RenderWorld(World& world);
};

