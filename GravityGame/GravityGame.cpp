#include "GravityGame.h"
#include <iostream>

namespace Gravity
{

Application::Application(GLFWwindow* w)
	: camera(), s(camera.getViewProj(), 0.0f, 0.0f, 0.0f, 0.7f, std::string("/sphere.shader"), std::string("/earth2.png"), 40, 18, 2),
	lightSource(camera.getViewProj(), 2.0f, 2.0f, -2.0f, 0.2f, std::string("/lightSource.shader"), std::string("/earth2.png"), 40, 18, 2),
	deltaTime(0.0f), lastFrame(0.0f), firstMouse(true), lastX(0.0f), lastY(0.0f)
{
	window = w;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	float r = 204, g = 57, b = 123;
	r /= 255.0f;
	g /= 255.0f;
	b /= 255.0f;
	lightSource.setUniform3f("lightColor", r,g,b);	
	s.setUniform3f("lightColor", r,g,b);
	s.setUniform3f("lightPos", lightSource.getCenter().x, lightSource.getCenter().y, lightSource.getCenter().z);
}


void Application::OnRender()
{
	renderer.Clear();
	s.draw();
	//s.drawLines();
	lightSource.draw();
	lightSource.drawLines();
}

void Application::OnUpdate()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	camera.update();
	s.update();
	lightSource.update();
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
	lightSource.rotateLeft();
	lightSource.updatePosition();
	s.setUniform3f("lightPos", lightSource.getCenter().x, lightSource.getCenter().y, lightSource.getCenter().z);
}

void Application::OnImGuiRender()
{
	if (menu)
	{
		ImGui::Begin("Move lightSource");
		ImGui::SliderFloat("X", &(lightSource.getCenter().x), -10.0f, 10.0f);
		ImGui::SliderFloat("Y", &(lightSource.getCenter().y), -10.0f, 10.0f);
		ImGui::SliderFloat("Z", &(lightSource.getCenter().z), -10.0f, 10.0f);
		ImGui::End();
	}
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ImGuiIO& io = ImGui::GetIO();
	if (action == GLFW_PRESS || action == GLFW_RELEASE) {
		ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
	}
	if (io.WantCaptureKeyboard) {
		//return; //if the input needs to be read only by ImGui
		//For now, it's better if all inputs get read by both GLFW and ImGui
	}
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		static bool open = false;
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		open = !open;
		app->menu = !app->menu;
		app->firstMouse = true;
		if (!open)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		app->camera.resetCamera();
	}
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
	ImGuiIO& io = ImGui::GetIO();

	io.AddMousePosEvent((float)xpos, (float)ypos);

	if (io.WantCaptureMouse) {
		return;  // ImGui is handling the mouse input, so return early
	}
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	if (app->menu)
		return;
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