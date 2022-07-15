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
#include "Shader.h"
#include "Model.h"
using namespace std;

// #define FIXEDUPDATE_TIME 0.01f//In seconds


constexpr int numVAOs = 1;
constexpr int numVBOs = 3;

float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

//分配在display函数中使用的变量空间，这样它们就不用在渲染的过程分配
GLuint mvLoc, proLoc;//视图矩阵和透视矩阵的指针
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, invMat;//透视、视图、模型、模型-视图矩阵
// 加载模型
ImportedModel myModel = ImportedModel(R"(.\Model\NasaShuttle\shuttle.obj)");

// Model* newModel;

// 纹理id
GLuint texMainId;

//model
glm::vec3 cubeScale(1.0f, 1.0f, 1.0f);

// lighting
glm::vec3 lightPos(1.0f, 1.0f, 1.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

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

#if 0
void setupVertics()
{
	//36个顶点，12个三角形，组成一个立方体
	float vertexPositions[108] = {
	-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
	1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
	1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};

	glGenVertexArrays(1, vao);//创建vao，返回整数型的id存进vao数组中
	glBindVertexArray(vao[0]);//将vao声明为活跃
	glGenBuffers(numVBOs, vbo);//创建vbo，返回整数型的id存进vbo数组中
	//将顶点信息放到缓冲区VBO中
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}
#endif

Shader* initialShader;
Shader* lightCubeShader;
Shader* modelShader;


GLuint lightCubeVAO;
GLuint lightCubeVBO;


void setupVertics()
{
	std::vector<glm::vec3> vert = myModel.getVertices();
	std::vector<glm::vec2> tex = myModel.getTextureCoords();
	std::vector<glm::vec3> norm = myModel.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	for (int i = 0; i < myModel.getNumVertices(); i++) {
		pvalues.push_back((vert[i]).x);
		pvalues.push_back((vert[i]).y);
		pvalues.push_back((vert[i]).z);
		tvalues.push_back((tex[i]).s);
		tvalues.push_back((tex[i]).t);
		nvalues.push_back((norm[i]).x);
		nvalues.push_back((norm[i]).y);
		nvalues.push_back((norm[i]).z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	// 发光立方体
	float vertices[108] = {
	-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
	1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
	1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};

	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glGenBuffers(1, &lightCubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

/*void generateModel(glm::vec3 pos)
{
	modelShader->use();
	modelShader->setMat4("projection", pMat);
	modelShader->setMat4("view", vMat);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pos); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	modelShader->setMat4("model", model);
	newModel->Draw(*modelShader);
}*/

void updateMenu()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

	ImGui::SliderFloat("float", &cubeLocY, 0.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)&lightColor); // Edit 3 floats representing a color
	float scale[3] = { 0.0f,0.0f,0.0f };
	auto objScale = mo->getScale();
	scale[0] = objScale.x;
	scale[1] = objScale.y;
	scale[2] = objScale.z;

	ImGui::DragFloat3("Scale", scale, 0.1f, 0, 5.0f, "%.6f");

	mo->setScale(glm::vec3(scale[0],scale[1],scale[2]));
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



	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	initialShader = new Shader("vertShader.glsl", "fragShader.glsl");
	lightCubeShader = new Shader("colorVert.glsl", "colorFrag.glsl");
	modelShader = new Shader("modelVert.glsl", "modelFrag.glsl");

	// newModel = new Model("Model/nanosuit/nanosuit.obj");

	// renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	pMat = CameraManager::instance()->getCurCamera()->getProjectionMatrix();
	cubeLocX = 0.0f; cubeLocY = 0.0f; cubeLocZ = 0.0f;
	setupVertics();
	texMainId = Utils::loadTexture(".\\Texture\\spstob_1.jpg");

	MeshManager::instance();
	TextureManager::Instance();

	mo = new MeshObject();
	mo->SetMeshData(MeshManager::instance()->getBuildInBox());
	mo->SetTexture(TextureManager::Instance()->LoadDefaultD());
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
	// glUseProgram(renderingProgram);



	/*测试 控制操作*/
	// updateController();
	ControllerManager::instance()->getCurController()->update();

	// //获取mv矩阵和投影矩阵的uniform
	// mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	// proLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	//创建视图矩阵，模型矩阵和视图-模型矩阵
	CameraManager::instance()->getCurCamera()->updateViewMatrix();
	vMat = CameraManager::instance()->getCurCamera()->getViewMatrix();

	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
	mMat = glm::scale(mMat, cubeScale);
	mvMat = vMat * mMat;

	// generateModel(glm::vec3(0.1f,0.1f,0.1f));
	mo->Render(modelShader);

	initialShader->use();
	// mMat = glm::rotate(mMat, 0.6f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	initialShader->setVec3("light.direction", -0.2f, -1.0f, -0.3f);
	initialShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	initialShader->setVec3("lightColor", lightColor);
	initialShader->setVec3("lightPos", lightPos);
	initialShader->setVec3("viewPos", CameraManager::instance()->getCurCamera()->getPosition());
	initialShader->setMat4("projection", pMat);
	initialShader->setMat4("view", vMat);
	initialShader->setMat4("model", mMat);


	//将透视矩阵和mv矩阵赋值给相应的uniform
	/*initialShader->setMat4("mv_matrix",mvMat);
	initialShader->setMat4("proj_matrix", pMat);
	initialShader->setVec3("lightColor", glm::vec3(0.2f, 0.4f, 1.0f));*/

	// glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	// glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	/*
	//将VBO关联给顶点着色器中相应的顶点属性
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//调整OpenGL设置，绘制模型
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);*/

	glBindVertexArray(vao[0]);
	//将VBO关联给顶点着色器中相应的顶点属性
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//绑定uv坐标
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	//激活纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texMainId);

	//绑定法向量
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	//调整OpenGL设置，绘制模型
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, myModel.getNumVertices());




	mMat = glm::translate(glm::mat4(1.0f), lightPos);
	mMat = glm::scale(mMat, glm::vec3(0.2f));
	mvMat = vMat * mMat;
	lightCubeShader->use();
	lightCubeShader->setMat4("proMatrix", pMat);
	lightCubeShader->setMat4("mvMatrix", mvMat);

	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);

}


int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	WindowManager::instance()->createMainWindow(1920, 1080, "MyTinyRender", false);

	/* 测试 控制代码*/
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


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(WindowManager::instance()->getWindow());
	glfwTerminate();
	exit(EXIT_SUCCESS);
}