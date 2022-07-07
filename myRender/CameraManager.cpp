//
// Filename: CameraManager.cpp
// Created by W. Mysterio on 2022-07-07 03:20:01.
// Description:
// Mail: woden3702@gmail.com
//

#include "CameraManager.h"

CameraManager* CameraManager::ins_ = nullptr;

CameraManager::CameraManager() = default;

CameraManager::~CameraManager()
{
	ins_ = nullptr;
}

CameraManager* CameraManager::instance()
{
	if (ins_ == nullptr)
		ins_ = new CameraManager();
	return ins_;
}

std::shared_ptr<Camera> CameraManager::push()
{
	std::shared_ptr<Camera> newCam = std::make_shared<Camera>();
	cams_.push(newCam);
	return newCam;
}

void CameraManager::pop()
{
	cams_.pop();
}

std::shared_ptr<Camera> CameraManager::getCurCamera()
{
	return cams_.top();
}
