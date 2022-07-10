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
using namespace std;

// #define FIXEDUPDATE_TIME 0.01f//In seconds


constexpr int numVAOs = 1;
constexpr int numVBOs = 3;

float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

//������display������ʹ�õı����ռ䣬�������ǾͲ�������Ⱦ�Ĺ��̷���
GLuint mvLoc, proLoc;//��ͼ�����͸�Ӿ����ָ��
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;//͸�ӡ���ͼ��ģ�͡�ģ��-��ͼ����
// ����ģ��
ImportedModel myModel = ImportedModel(R"(.\Model\NasaShuttle\shuttle.obj)");

// ����id
GLuint texMainId;


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
	//36�����㣬12�������Σ����һ��������
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

	glGenVertexArrays(1, vao);//����vao�����������͵�id���vao������
	glBindVertexArray(vao[0]);//��vao����Ϊ��Ծ
	glGenBuffers(numVBOs, vbo);//����vbo�����������͵�id���vbo������
	//��������Ϣ�ŵ�������VBO��
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}
#endif


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
}



void init()
{
	CameraManager::instance()->push();
	ControllerManager::instance()->push();

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
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
	glUseProgram(renderingProgram);

	/*���� ���Ʋ���*/
	// updateController();
	ControllerManager::instance()->getCurController()->update();

	//��ȡmv�����ͶӰ�����uniform
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	proLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	//������ͼ����ģ�;������ͼ-ģ�;���
	CameraManager::instance()->getCurCamera()->updateViewMatrix();
	vMat = CameraManager::instance()->getCurCamera()->getViewMatrix();

	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
	// mMat = glm::rotate(mMat, 0.6f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	mvMat = vMat * mMat;

	//��͸�Ӿ����mv����ֵ����Ӧ��uniform
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(proLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	/*
	//��VBO������������ɫ������Ӧ�Ķ�������
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//����OpenGL���ã�����ģ��
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);*/

	//��VBO������������ɫ������Ӧ�Ķ�������
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//��uv����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	//��������
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texMainId);

	//����OpenGL���ã�����ģ��
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, myModel.getNumVertices());

}


int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	WindowManager::instance()->createMainWindow(1920, 1080, "test_1", false);
	

	/* ���� ���ƴ���*/
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