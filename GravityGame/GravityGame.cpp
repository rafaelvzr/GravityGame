#include "GravityGame.h"
#include <iostream>

float aleatorio()
{
	float r = (float) rand() / RAND_MAX;
	r *= 240.0f;
	r -= 120.0f;
	return r;
}

namespace Gravity
{

Application::Application(GLFWwindow* w)
	: camera(), deltaTime(0.0f), lastFrame(0.0f), firstMouse(true), lastX(0.0f), lastY(0.0f), marker(camera), grid(camera), 
	portal(camera, 20.0f,22.0f,-60.0f,-120.0f), createNewScene(false)
{
	window = w;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	Planet* p1 = new Planet(-20.0f, 40.0f, 50.0f, 2.0f, 10.0f, true, camera);
	Star* st = new Star(0.0f, 0.0f, -3.0f, 6.0f, 1000.0f,false, camera, "/lightSource2.shader");

	p1->setVelocity(0.5f,-0.5f,-0.3f);

	celestialBodies.push_back(p1);
	celestialBodies.push_back(st);
}


void Application::OnRender()
{
	renderer.Clear();
	grid.draw();
	marker.draw();
	for (size_t i = 0; i < celestialBodies.size(); ++i)
	{
		celestialBodies[i]->draw();
	}
	portal.draw();
}

void Application::OnUpdate()
{
	camera.update();
	grid.update();
	marker.update();
	portal.update(deltaTime);
	if (walkingFoward)
		camera.translateCamera(FORWARD, deltaTime);
	if (walkingBackward)
		camera.translateCamera(BACKWARD, deltaTime);
	if (walkingLeft)
		camera.translateCamera(LEFT, deltaTime);
	if (walkingRight)
		camera.translateCamera(RIGHT, deltaTime);
	if (markerFront)
		marker.moveZAxis(50.0f, deltaTime);
	if (markerBack)
		marker.moveZAxis(-50.0f, deltaTime);
	if (markerLeft)
		marker.moveXAxis(-50.0f, deltaTime);
	if (markerRight)
		marker.moveXAxis(50.0f, deltaTime);
	if (markerUp)
		marker.moveYAxis(50.0f, deltaTime);
	if (markerDown)
		marker.moveYAxis(-50.0f, deltaTime);
	if (rotateClockwise)
		camera.rotateScene(50.0f, deltaTime);
	if (rotateCounterClockwise)
		camera.rotateScene(-50.0f, deltaTime);
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	if (!pause) 
	{
		for (size_t i = 0; i < celestialBodies.size(); ++i)
		{
			for (size_t j = 0; j < celestialBodies.size(); ++j)
			{
				if (i != j)
					if (celestialBodies[i]->getMovable())
						celestialBodies[i]->getAttracted(*celestialBodies[j]);
			}
		}
		for (size_t i = 0; i < celestialBodies.size(); ++i)
			celestialBodies[i]->update(deltaTime);
	}
	for (size_t i = 0; i < celestialBodies.size(); ++i)
		celestialBodies[i]->updateVP();
	float d = glm::distance(celestialBodies[0]->getPosition(), portal.getCenter());
	if (d < 60.0f)
		createNewScene = true;
	if (createNewScene)
	{
		celestialBodies.clear();
		Planet* p1 = new Planet(aleatorio(), aleatorio(), aleatorio(), 2.0f, 10.0f, true, camera);
		Star* st = new Star(aleatorio(), aleatorio(), aleatorio(), 6.0f, 1000.0f, false, camera, "/lightSource2.shader");
		celestialBodies.push_back(p1);
		celestialBodies.push_back(st);
		portal.setPosition(aleatorio(), aleatorio(), aleatorio());
		createNewScene = false;
	}
}
void Application::OnImGuiRender()
{
	if (menu)
	{
		ImGui::Begin("Menu", &menu);
		if (ImGui::Button("Pause"))
			pause = !pause;
		if (ImGui::TreeNode("Camera"))
		{
			ImGui::Text("x: %f y: %f z:%f", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Gravidade"))
		{
			ImGui::Text("x: %f y: %f z:%f", celestialBodies[0]->getPosition().x, celestialBodies[0]->getPosition().y, celestialBodies[0]->getPosition().z);
			ImGui::TreePop();
		}
		ImGui::End();
	}
}



void Application::createPlanet(float radius, float mass)
{
	Planet* p = new Planet(marker.getPosition(), radius, mass,true, camera);
	celestialBodies.push_back(p);
}

void Application::createYellowStar(float radius, float mass)
{
	Star* s = new Star(marker.getPosition(), radius, mass,false, camera, "/lightSource2.shader");
	celestialBodies.push_back(s);
}

void Application::createBlueStar(float radius, float mass)
{
	Star* s = new Star(marker.getPosition(), radius, mass, false, camera, "/lightSource3.shader");
	celestialBodies.push_back(s);
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
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		app->pause = !app->pause;
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		app->rotateClockwise = true;
	}
	if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
	{
		app->rotateClockwise = false;
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		app->rotateCounterClockwise = true;
	}
	if (key == GLFW_KEY_E && action == GLFW_RELEASE)
	{
		app->rotateCounterClockwise = false;
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		app->createYellowStar(5.0f, 100.0f);
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		app->createBlueStar(25.0f, 500.0f);
	}
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

	if(key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		app->markerFront = true;
	}
	if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
	{
		app->markerFront = false;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		app->markerBack = true;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
	{
		app->markerBack = false;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		app->markerLeft = true;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
	{
		app->markerLeft = false;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		app->markerRight = true;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
	{
		app->markerRight = false;
	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		app->markerUp = true;
	}
	if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
	{
		app->markerUp = false;
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		app->markerDown = true;
	}
	if (key == GLFW_KEY_X && action == GLFW_RELEASE)
	{
		app->markerDown = false;
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