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



namespace Gravity {
class Application 
{
private:
	Renderer renderer;
	GLFWwindow* window;
	Camera camera;

	Sphere s;
	Sphere lightSource;
	float deltaTime;
	float lastFrame;
	float lastX;
	float lastY;
	bool firstMouse;
	bool menu = false;
public:
	bool rotateLeft = false;
	bool rotateRight = false;
	bool walkingFoward = false;
	bool walkingBackward = false;
	bool walkingLeft = false;
	bool walkingRight = false;
	Application(GLFWwindow* w);
	//~Application();
	void OnRender();
	void OnUpdate();	
	void OnImGuiRender();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

}
