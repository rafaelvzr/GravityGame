#include <math.h>

#include"imgui.h"
#include"imgui_impl_opengl3.h"
#include <imgui_impl_glfw.h>

#include"glm.hpp"
#include"gtc/matrix_transform.hpp"

#include <iostream>
#include<GravityGame.h>
#include <GLFW/glfw3.h>



int main(int argc, char **argv)
{
	if(!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE); //transparent background

	GLFWwindow *window = glfwCreateWindow(1024, 768, "StockChart", nullptr, nullptr);

	if(!window){
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);


	GLCall(glEnable(GL_BLEND));
	glEnable(GL_DEPTH_TEST);
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	glfwSwapInterval(1);
	ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;     
	ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");
	ImGuiWindowFlags debugWindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);

	{
		Gravity::Application app(window);
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			//OnUpdate, OnRender and OnImGuiRender() functions
			app.OnUpdate();
			app.OnRender();
			app.OnImGuiRender();
			ImGui::Begin("Framerate", nullptr, debugWindowFlags);
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			GLCall(glfwSwapBuffers(window));
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}




