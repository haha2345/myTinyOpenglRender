//
// Filename: ControllerManager.cpp
// Created by W. Mysterio on 2022-07-07 04:20:43.
// Description:
// Mail: woden3702@gmail.com
//

#include "ControllerManager.h"
ControllerManager* ControllerManager::ins_ = nullptr;

ControllerManager::ControllerManager()= default;

ControllerManager::~ControllerManager()
{
	ins_ = nullptr;
}

ControllerManager* ControllerManager::instance()
{
	if (ins_ == nullptr)
		ins_ = new ControllerManager();
	return ins_;
}

void ControllerManager::push()
{
	auto newCtrl = std::make_shared<Controller>();
	ctrls_.push(newCtrl);
}

void ControllerManager::push(std::shared_ptr<Controller> controller)
{
	ctrls_.push(controller);
}

void ControllerManager::pop()
{
	ctrls_.pop();
}

std::shared_ptr<Controller> ControllerManager::getCurController()
{
	return ctrls_.top();
}
