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
#include "Utils.h"
#include "WindowManager.h"
#include "CameraManager.h"
#include "ControllerManager.h"
#include "ImportedModel.h"
#include "Shader.h"
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

// 纹理id
GLuint texMainId;



// lighting
glm::vec3 lightPos(1.0f, 1.0f, 1.0f);

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

/*void installLights(glm::mat4 vMatrix) {
	//将光源位置转换为视图空间坐标
	lightPosV = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
	lightPos[0] = lightPosV.x;
	lightPos[1] = lightPosV.y;
	lightPos[2] = lightPosV.z;

	initialShader->use();

	initialShader->setVec4("globalAmbient", glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
	initialShader->setVec4("light.ambient", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	initialShader->setVec4("light.diffuse", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	initialShader->setVec4("light.specular", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	initialShader->setVec3("light.position", glm::vec3(lightPos[0], lightPos[1], lightPos[2]));
	//银材质
	initialShader->setVec4("material.ambient", glm::vec4(0.2473f, 0.1995f, 0.0745f, 1));
	initialShader->setVec4("material.diffuse", glm::vec4(0.7516f, 0.6065f, 0.2265f, 1));
	initialShader->setVec4("material.specular", glm::vec4(0.6283f, 0.5559f, 0.3661f, 1));
	initialShader->setFloat("material.shininess", 51.2f);

}*/

void init()
{
	CameraManager::instance()->push();
	ControllerManager::instance()->push();

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	initialShader = new Shader("vertShader.glsl", "fragShader.glsl");
	lightCubeShader = new Shader("colorVert.glsl", "colorFrag.glsl");

	// renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	pMat = CameraManager::instance()->getCurCamera()->getProjectionMatrix();
	cubeLocX = 0.0f; cubeLocY = 0.0f; cubeLocZ = 0.0f;
	setupVertics();
	texMainId = Utils::loadTexture(".\\Texture\\spstob_1.jpg");
}


void display(double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	// glUseProgram(renderingProgram);

	initialShader->use();


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
	// mMat = glm::rotate(mMat, 0.6f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	initialShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	initialShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	initialShader->setVec3("lightPos", lightPos);
	initialShader->setVec3("viewPos", CameraManager::instance()->getCurCamera()->getPosition());
	initialShader->setMat4("projection", pMat);
	initialShader->setMat4("view", vMat);
	initialShader->setMat4("model", mMat);

	mvMat = vMat * mMat;

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

	WindowManager::instance()->createMainWindow(1920, 1080, "test_1", false);
	

	/* 测试 控制代码*/
	glfwSetMouseButtonCallback(WindowManager::instance()->getWindow(), MouseKeyCallback);
	glfwSetKeyCallback(WindowManager::instance()->getWindow(), KeyInputCallback);
	glfwSetCursorPosCallback(WindowManager::instance()->getWindow(), MouseMotionCallback);


	init();

	while (!glfwWindowShouldClose(WindowManager::instance()->getWindow())) {
		display(glfwGetTime());
		glfwSwapBuffers(WindowManager::instance()->getWindow());
		glfwPollEvents();
	}

	glfwDestroyWindow(WindowManager::instance()->getWindow());
	glfwTerminate();
	exit(EXIT_SUCCESS);
}