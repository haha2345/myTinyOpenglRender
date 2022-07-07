//
// Filename: CameraManager.h
// Created by W. Mysterio on 2022-07-07 03:20:01.
// Description:
// Mail: woden3702@gmail.com
//

#ifndef __CAMERAMANAGER_H__
#define __CAMERAMANAGER_H__
#include <stack>
#include <memory>

#include "Camera.h"

class CameraManager
{
private:
	CameraManager();
	static CameraManager* ins_;
	std::stack<std::shared_ptr<Camera>> cams_;
public:

	~CameraManager();
	static CameraManager* instance();
	std::shared_ptr<Camera> push();
	void pop();
	std::shared_ptr<Camera> getCurCamera();
};

#endif //__CAMERAMANAGER_H__


