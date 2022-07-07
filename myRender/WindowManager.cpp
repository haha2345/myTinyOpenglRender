//
// Filename: WindowManager.cpp
// Created by W. Mysterio on 2022-07-04 02:01:39.
// Description:
// Mail: woden3702@gmail.com
//

#include "WindowManager.h"

WindowManager* WindowManager::ins_ = nullptr;

WindowManager::WindowManager() = default;


WindowManager::~WindowManager()
{
	delete ins_;
	ins_ = nullptr;
}

WindowManager* WindowManager::instance()
{
	if (ins_ == nullptr)
		ins_ = new WindowManager();
	return ins_;
}

void WindowManager::createMainWindow(unsigned w, unsigned h, const char* title, bool fullScreen)
{
	wSize_.w = w;
	wSize_.h = h;
	if (fullScreen)
		window_ = glfwCreateWindow(static_cast<int>(wSize_.w), static_cast<int>(wSize_.h), title, glfwGetPrimaryMonitor(), nullptr);
	else
		window_ = glfwCreateWindow(static_cast<int>(wSize_.w), static_cast<int>(wSize_.h), title, nullptr, nullptr);

	glfwSetWindowAttrib(window_, GLFW_RESIZABLE, GLFW_FALSE);//控制是否缩放
	glfwMakeContextCurrent(window_);
	glViewport(0, 0, static_cast<GLsizei>(wSize_.w), static_cast<GLsizei>(wSize_.h));
}

WindowSize WindowManager::getWindowSize() const
{
	return wSize_;
}

GLFWwindow* WindowManager::getWindow() const
{
	return window_;
}

