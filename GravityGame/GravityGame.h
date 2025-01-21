#pragma once
#include"renderer.h"
#include"VertexBufferLayout.h"
#include"Texture.h"
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>
#include"imgui.h"
#include"Sphere.h"
#include"Camera.h"
#include <imgui_impl_glfw.h>
#include <CelestialBody.h>



namespace Gravity {
class Application 
{
private:
	Renderer renderer;
	GLFWwindow* window;
	Camera camera;
	
	Marker marker;
	Grid grid;
	Portal portal;
	float deltaTime;
	float lastFrame;
	float lastX;
	float lastY;
	bool firstMouse;
	bool menu = false;
	bool pause = false;
	std::vector<CelestialBody*> celestialBodies;
	bool createNewScene;
	bool walkingFoward = false;
	bool walkingBackward = false;
	bool walkingLeft = false;
	bool walkingRight = false;
	bool markerFront = false;
	bool markerBack = false;
	bool markerLeft = false;
	bool markerRight = false;
	bool markerUp = false;
	bool markerDown = false;
	bool rotateClockwise = false;
	bool rotateCounterClockwise = false;
public:
	Application(GLFWwindow* w);
	//~Application();
	void OnRender();
	void OnUpdate();	
	void OnImGuiRender();

	void createPlanet(float radius, float mass);
	void createYellowStar(float radius, float mass);
	void createBlueStar(float radius, float mass);

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

}
