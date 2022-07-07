//
// Filename: WindowManager.h
// Created by W. Mysterio on 2022-07-04 02:01:39.
// Description:
// Mail: woden3702@gmail.com
//

#ifndef __WINDOWMANAGER_H__
#define __WINDOWMANAGER_H__
#include "GLFW/glfw3.h"

struct WindowSize
{
	unsigned int w;//width
	unsigned int h;//height
};

class WindowManager
{
private:
	static WindowManager* ins_;
	WindowManager();
	WindowSize wSize_;
	GLFWwindow* window_;
public:
	~WindowManager();
	static WindowManager* instance();
	void createMainWindow(unsigned int w, unsigned int h, const char* title, bool fullScreen);
	WindowSize getWindowSize() const;
	GLFWwindow* getWindow() const;
};

#endif //__WINDOWMANAGER_H__


