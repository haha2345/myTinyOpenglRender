//
// Filename: ControllerManager.h
// Created by W. Mysterio on 2022-07-07 04:20:43.
// Description:
// Mail: woden3702@gmail.com
//

#ifndef __CONTROLLERMANAGER_H__
#define __CONTROLLERMANAGER_H__
#include <stack>
#include <memory>
#include "Controller.h"

class ControllerManager
{
private:
	ControllerManager();
	static ControllerManager* ins_;
	std::stack<std::shared_ptr<Controller>> ctrls_;
public:
	~ControllerManager();
	static ControllerManager* instance();
	void push();
	void push(std::shared_ptr<Controller>);
	void pop();
	std::shared_ptr<Controller> getCurController();
};

#endif //__CONTROLLERMANAGER_H__


