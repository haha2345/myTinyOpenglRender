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
#include "MenuManager.h"
#include "TerrainManager.h"
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



void init()
{
	
	CameraManager::instance()->push();
	ControllerManager::instance()->push();
	FileImportManager::instance();

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	MenuManager::instance()->initMenu();

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
	TerrainManager::instance();

}



void display(double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	ControllerManager::instance()->getCurController()->update();

	//创建视图矩阵，模型矩阵和视图-模型矩阵
	CameraManager::instance()->getCurCamera()->updateViewMatrix();
	MenuManager::instance()->updateMenu();

	light->setPosition(glm::vec3(1,1,1));
	initialShader->use();
	initialShader->setVec3("viewPos", CameraManager::instance()->getCurCamera()->getPosition());
	initialShader->setVec3("light.position", glm::vec3(1, 1, 1));
	initialShader->setVec3("light.direction", -1, -1.0f, -1);

	initialShader->setVec3("light.ambient", 0.0f, 0.0f, 0.0f);
	initialShader->setVec3("light.diffuse", 1.0f, 1.0f,1.0f);
	initialShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	initialShader->setFloat("light.constant", 1.0f);
	initialShader->setFloat("light.linear", 0.09f);
	initialShader->setFloat("light.quadratic", 0.032f);
	initialShader->setFloat("material.shininess", 32.0f);


	mo->render(modelShader);

	// FileImportManager::instance()->show(initialShader);

	SceneManager::instance()->draw(initialShader);
	TerrainManager::instance()->getTerrainObject()->render(initialShader);
	TerrainManager::instance()->getTerrainObject()->update(CameraManager::instance()->getCurCamera().get());


	//调整OpenGL设置，绘制模型
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	MenuManager::instance()->renderMenu();
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

	while (!glfwWindowShouldClose(WindowManager::instance()->getWindow())) {
		display(glfwGetTime());
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