//
// Filename: Controller.h
// Created by W. Mysterio on 2022-07-07 03:58:35.
// Description:
// Mail: woden3702@gmail.com
//

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include <glm/vec3.hpp>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include "WindowManager.h"
#define FIXEDUPDATE_TIME 0.01f//In seconds

class Controller
{
private:
	double lastMousePos[2];
	int curMouseButton;
	float moveSpeed=3.0f;
	glm::vec3 moveVec;
	float rotSensitivity[2];
	float mouseMovSensitivity[3];
	float offsetX, offsetY;
	float yaw, pitch;//两种欧拉角  俯仰角(Pitch)、偏航角(Yaw)
public:
	Controller();
	void init();
	void update();
	void MouseMotionCallback(GLFWwindow* window, double x, double y);
	void MouseKeyCallback(GLFWwindow* window, int button, int state, int mods);
	void KeyInputCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
};

#endif //__CONTROLLER_H__


