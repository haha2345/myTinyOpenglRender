#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm\glm.hpp>
#include <memory>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Utils.h"
#include "WindowManager.h"
#include "CameraManager.h"
#include "ControllerManager.h"
#include "MeshObject.h"
#include "ImportedModel.h"
#include "FileImportManager.h"
#include "Shader.h"
#include "Light.h"
#include "SceneManager.h"
using namespace std;

// #define FIXEDUPDATE_TIME 0.01f//In seconds


MeshObject* mo;


void MouseMotionCallback(GLFWwindow* window, double x, double y)
{
	ControllerManager::instance()->getCurController()->MouseMotionCallback(window, x, y);
}
void MouseKeyCallback(GLFWwindow* window, int button, int state, int mods)
{
	ControllerManager::instance()->getCurController()->MouseKeyCallback(window, button, state, mods);
}
void KeyInputCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	ControllerManager::instance()->getCurController()->KeyInputCallback(window, key, scanCode, action, mods);
}

Shader* initialShader;
Shader* lightCubeShader;
Shader* modelShader;

Light* light;

glm::vec3 pos(0,0,0);

void updateMenu()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

	float scale[3] = { 0.0f,0.0f,0.0f };
	auto objScale = mo->getScale();
	scale[0] = objScale.x;
	scale[1] = objScale.y;
	scale[2] = objScale.z;

	ImGui::DragFloat3("Scale", scale, 0.1f, 0, 5.0f, "%.6f");
	mo->setScale(glm::vec3(scale[0], scale[1], scale[2]));

	float position[3] = { 0.0f,0.0f,0.0f };
	auto objPos = mo->getPosition();
	position[0] = objPos.x;
	position[1] = objPos.y;
	position[2] = objPos.z;
	ImGui::DragFloat3("position", position, 0.1, -5.0f, 5.0f, "%.6f");
	mo->setPosition(glm::vec3(position[0], position[1], position[2]));

	if (ImGui::Button("Use", ImVec2(60, 20)))
	{
		FileImportManager::instance()->loadFile("Model/nanosuit/nanosuit.obj");
	}

	// if (ImGui::Button("Add Model"))
		// generateModel(glm::vec3(1.0f,1.0f,1.0f));

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void init()
{
	
	CameraManager::instance()->push();
	ControllerManager::instance()->push();
	FileImportManager::instance();

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	initialShader = new Shader("blinnVert.glsl", "blinnFrag.glsl");
	lightCubeShader = new Shader("colorVert.glsl", "colorFrag.glsl");
	modelShader = new Shader("modelVert.glsl", "modelFrag.glsl");

	MeshManager::instance();
	TextureManager::instance();

	light = new Light(LightType::Point, 100.0f);

	mo = new MeshObject();
	mo->setMeshData(MeshManager::instance()->getBuildInBox());
	mo->setTexture(TextureManager::instance()->loadDefaultD());
	mo->setMeshType(MeshType::buildIn);
	mo->setPosition(glm::vec3(1.0, 1.0, 1.0));
	mo->setScale(glm::vec3(0.1f, 0.1f, 0.1f));

	SceneManager::instance();

}

void InitGui()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(WindowManager::instance()->getWindow(), true);
	ImGui_ImplOpenGL3_Init();
}

void display(double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	ControllerManager::instance()->getCurController()->update();

	//创建视图矩阵，模型矩阵和视图-模型矩阵
	CameraManager::instance()->getCurCamera()->updateViewMatrix();

	light->setPosition(mo->getPosition());
	initialShader->use();
	initialShader->setVec3("light.position",light->getPosition());
	initialShader->setVec3("light.direction", -0.2f, -1.0f, -0.3f);

	initialShader->setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
	initialShader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
	initialShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	initialShader->setFloat("light.constant", 1.0f);
	initialShader->setFloat("light.linear", 0.09f);
	initialShader->setFloat("light.quadratic", 0.032f);
	initialShader->setFloat("material.shininess", 10.0f);


	mo->render(modelShader);

	// FileImportManager::instance()->show(initialShader);

	SceneManager::instance()->draw(initialShader);

	//调整OpenGL设置，绘制模型
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}


int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	WindowManager::instance()->createMainWindow(1920, 1080, "MyTinyRender", false);

	glfwSetMouseButtonCallback(WindowManager::instance()->getWindow(), MouseKeyCallback);
	glfwSetKeyCallback(WindowManager::instance()->getWindow(), KeyInputCallback);
	glfwSetCursorPosCallback(WindowManager::instance()->getWindow(), MouseMotionCallback);

	init();

	InitGui();

	while (!glfwWindowShouldClose(WindowManager::instance()->getWindow())) {
		display(glfwGetTime());
		updateMenu();
		glfwSwapBuffers(WindowManager::instance()->getWindow());
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown;
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(WindowManager::instance()->getWindow());
	glfwTerminate();
	exit(EXIT_SUCCESS);
}