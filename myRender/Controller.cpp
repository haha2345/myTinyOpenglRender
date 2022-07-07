//
// Filename: Controller.cpp
// Created by W. Mysterio on 2022-07-07 03:58:35.
// Description:
// Mail: woden3702@gmail.com
//

#include "Controller.h"

#include "CameraManager.h"

Controller::Controller()
{
	init();
}

void Controller::init()
{
	rotSensitivity[0] = 0.05f;
	rotSensitivity[1] = 0.03f;
	mouseMovSensitivity[0] = 0.02f;
	mouseMovSensitivity[1] = 0.01f;
	mouseMovSensitivity[2] = 0.1f;
	moveVec = glm::vec3(0.0f, 0.0f, 0.0f);
	yaw = -90.0f;
	pitch = 0.0f;
}

void Controller::update()
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

void Controller::MouseMotionCallback(GLFWwindow* window, double x, double y)
{
	switch (curMouseButton)
	{
	case GLFW_MOUSE_BUTTON_RIGHT:
	{
			//参考https://learnopengl-cn.github.io/01%20Getting%20started/09%20Camera/#_6
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
	//记录鼠标松开的位置
	lastMousePos[0] = x;
	lastMousePos[1] = y;
}

void Controller::MouseKeyCallback(GLFWwindow* window, int button, int state, int mods)
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

void Controller::KeyInputCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
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
		if (action == GLFW_PRESS)
		{
			//回到原位
			CameraManager::instance()->getCurCamera()->setPosition(glm::vec3(0.0f, 0.0f, 8.0f));
			CameraManager::instance()->getCurCamera()->setDirection(glm::vec3(0.0f, 0.0f, -1.0f));
			CameraManager::instance()->getCurCamera()->updateViewMatrix();
		}
	}
}
