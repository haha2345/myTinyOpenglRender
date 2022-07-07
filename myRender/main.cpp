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
using namespace std;

// #define FIXEDUPDATE_TIME 0.01f//In seconds


constexpr int numVAOs = 1;
constexpr int numVBOs = 2;

float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

//分配在display函数中使用的变量空间，这样它们就不用在渲染的过程分配
GLuint mvLoc, proLoc;//视图矩阵和透视矩阵的指针
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;//透视、视图、模型、模型-视图矩阵

#if false
/*测试 控制器*/
double lastMousePos[2];
int curMouseButton;
float moveSpeed = 20.0f;
glm::vec3 moveVec(0.0f, 0.0f, 0.0f);

glm::vec3 forword(0.0f, 0.0f, -1.0f);
glm::vec3 camRight(1.0f, 0.0f, 0.0f);

float rotSensitivity[2] = { 0.02f, 0.02f };
float mouseMovSensitivity[3] = { 0.02f, 0.01f, 0.01f };
float offsetX, offsetY;
float yaw = -90.0f, pitch = 0.0f;

void MouseMotionCallback(GLFWwindow* window, double x, double y)
{
	switch (curMouseButton)
	{
	case GLFW_MOUSE_BUTTON_RIGHT:
	{
		offsetX = x - lastMousePos[0];
		offsetY = lastMousePos[1] - y;
		// printf("(%f,%f)\n", x, y);
		// printf("(%f,%f)\n", offsetX, offsetY);
		offsetX *= rotSensitivity[0];
		offsetY *= rotSensitivity[1];
		yaw += offsetX;
		pitch += offsetY;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		CameraManager::instance()->getCurCamera()->setDirection(normalize(front));
	}
	break;
	}
	lastMousePos[0] = x;
	lastMousePos[1] = y;
}

void MouseKeyCallback(GLFWwindow* window, int button, int state, int mods)
{
	switch (state)
	{
	case GLFW_PRESS:
	{
		curMouseButton = button;
	}
	break;
	case GLFW_RELEASE:
	{
		curMouseButton = -1;
	}
	break;
	}

	glfwGetCursorPos(window, &lastMousePos[0], &lastMousePos[1]);
}

void KeyInputCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	switch (key)
	{
	default:
		break;
	case GLFW_KEY_W:
		moveVec.x = (action == GLFW_PRESS || action == GLFW_REPEAT) ? 1.0f : 0.0f;
		break;
	case GLFW_KEY_S:
		moveVec.x = (action == GLFW_PRESS || action == GLFW_REPEAT) ? -1.0f : 0.0f;
		break;
	case GLFW_KEY_A:
		moveVec.y = (action == GLFW_PRESS || action == GLFW_REPEAT) ? -1.0f : 0.0f;
		break;
	case GLFW_KEY_D:
		moveVec.y = (action == GLFW_PRESS || action == GLFW_REPEAT) ? 1.0f : 0.0f;
		break;
	case GLFW_KEY_E:
		moveVec.z = (action == GLFW_PRESS || action == GLFW_REPEAT) ? 1.0f : 0.0f;
		break;
	case GLFW_KEY_Q:
		moveVec.z = (action == GLFW_PRESS || action == GLFW_REPEAT) ? -1.0f : 0.0f;
		break;
	}


	switch (key)
	{
	default:
		break;
	case GLFW_KEY_W:
		if (action == GLFW_RELEASE)
		{
			moveVec.x = 0.0f;
		}
		break;
	case GLFW_KEY_S:
		if (action == GLFW_RELEASE)
		{
			moveVec.x = 0.0f;
		}
		break;
	case GLFW_KEY_A:
		if (action == GLFW_RELEASE)
		{
			moveVec.y = 0.0f;
		}
		break;
	case GLFW_KEY_D:
		if (action == GLFW_RELEASE)
		{
			moveVec.y = 0.0f;
		}
		break;
	case GLFW_KEY_E:
		if (action == GLFW_RELEASE)
		{
			moveVec.z = 0.0f;
		}
		break;
	case GLFW_KEY_Q:
		if (action == GLFW_RELEASE)
		{
			moveVec.z = 0.0f;
		}
		break;
	case GLFW_KEY_SPACE:
		if (action==GLFW_PRESS)
		{
			//回到原位
			CameraManager::instance()->getCurCamera()->setPosition(glm::vec3(0.0f, 0.0f, 8.0f));
			CameraManager::instance()->getCurCamera()->setDirection(glm::vec3(0.0f, 0.0f, -1.0f));
			CameraManager::instance()->getCurCamera()->updateViewMatrix();

		}
	}
}

void updateController()
{
	if (curMouseButton == GLFW_MOUSE_BUTTON_RIGHT)
	{
		glm::vec3 curPos = CameraManager::instance()->getCurCamera()->getPosition();
		// printf("按下右键 position: ");
		// printf("(%f,%f,%f)\n", curPos.x, curPos.y, curPos.z);
		// printf("sasd%f\n", moveVec.x);
		curPos = curPos + (normalize(CameraManager::instance()->getCurCamera()->getDirection()) * moveSpeed * moveVec.x +
			normalize(CameraManager::instance()->getCurCamera()->getRight()) * moveSpeed * moveVec.y +
			normalize(CameraManager::instance()->getCurCamera()->getUp()) * moveSpeed * moveVec.z) * FIXEDUPDATE_TIME;
		// printf("______处理完后： ");
		// printf("(%f,%f,%f)\n", curPos.x, curPos.y, curPos.z);
		CameraManager::instance()->getCurCamera()->setPosition(curPos);
	}
}

#endif


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

void init(GLFWwindow* window)
{
	CameraManager::instance()->push();
	ControllerManager::instance()->push();

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);


	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	pMat = CameraManager::instance()->getCurCamera()->getProjectionMatrix();
	cubeLocX = 0.0f; cubeLocY = 0.0f; cubeLocZ = -3.0f;
	setupVertics();
}


void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);

	/*测试 控制操作*/
	// updateController();
	ControllerManager::instance()->getCurController()->update();

	//获取mv矩阵和投影矩阵的uniform
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	proLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	//创建视图矩阵，模型矩阵和视图-模型矩阵
	CameraManager::instance()->getCurCamera()->updateViewMatrix();
	vMat = CameraManager::instance()->getCurCamera()->getViewMatrix();
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
	mMat = glm::rotate(mMat, 0.6f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	mvMat = vMat * mMat;

	//将透视矩阵和mv矩阵赋值给相应的uniform
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	//将VBO关联给顶点着色器中相应的顶点属性
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//调整OpenGL设置，绘制模型
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


	init(WindowManager::instance()->getWindow());

	while (!glfwWindowShouldClose(WindowManager::instance()->getWindow())) {
		display(WindowManager::instance()->getWindow(), glfwGetTime());
		glfwSwapBuffers(WindowManager::instance()->getWindow());
		glfwPollEvents();
	}

	glfwDestroyWindow(WindowManager::instance()->getWindow());
	glfwTerminate();
	exit(EXIT_SUCCESS);
}