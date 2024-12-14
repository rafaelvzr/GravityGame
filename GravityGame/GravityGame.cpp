#include "GravityGame.h"
#include <iostream>

namespace Gravity
{

Application::Application(GLFWwindow* w)
	: camera(), s(camera.getViewProj(), 1.0f, 40, 18, 2), deltaTime(0.0f), lastFrame(0.0f), firstMouse(true), lastX(0.0f), lastY(0.0f)
{
	window = w;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

}

void Application::OnRender()
{
	renderer.Clear();
	s.draw();
	s.drawLines();
}

void Application::OnUpdate()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	camera.update();
	s.rotateLeft();
	s.update();
	if (walkingFoward)
		camera.translateCamera(FORWARD, deltaTime);
	if (walkingBackward)
		camera.translateCamera(BACKWARD, deltaTime);
	if (walkingLeft)
		camera.translateCamera(LEFT, deltaTime);
	if (walkingRight)
		camera.translateCamera(RIGHT, deltaTime);
	if (rotateLeft)
		s.rotateLeft();
	if (rotateRight)
		s.rotateRight();
}

void Application::OnImGuiRender()
{
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		app->rotateLeft = true;
	if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
		app->rotateLeft = false;
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
		app->rotateRight = true;
	if (key == GLFW_KEY_E && action == GLFW_RELEASE)
		app->rotateRight = false;
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		app->walkingFoward = true;
	}
	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		app->walkingFoward = false;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		app->walkingBackward = true;
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		app->walkingBackward = false;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		app->walkingLeft = true;
	}
	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
	{
		app->walkingLeft = false;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		app->walkingRight = true;
	}
	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
	{
		app->walkingRight = false;
	}
}

void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	if (app->firstMouse)
	{
		app->lastX = (float)xpos;
		app->lastY = (float)ypos;
		app->firstMouse = false;
	}
	float xoffset = xpos - app->lastX;
	float yoffset = app->lastY - ypos; // reversed since y-coordinates range from bottom to top
	app->lastX = xpos;
	app->lastY = ypos;
	app->camera.rotateCamera(xoffset, yoffset);
}

void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->camera.zoomCamera(yoffset);
}

}