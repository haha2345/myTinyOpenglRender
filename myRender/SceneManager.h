//
// Filename: SceneManager.h
// Created by W. Mysterio on 2022-07-24 09:38:26.
// Description:
// Mail: woden3702@gmail.com
//

#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__
#include <list>
#include <memory>
#include "Object.h"
#include "MeshObject.h"
#include "Light.h"
#include "Shader.h"

class SceneManager
{
private:
	SceneManager();
	static SceneManager* ins_;

	std::list<std::shared_ptr<Object>> objects_;
public:
	~SceneManager();
	static SceneManager* instance();

	void init();

	void draw(Shader* shader);

	void deleteObject(std::shared_ptr<Object> obj);

	void addObject(std::shared_ptr<Object> obj);

	
};


#endif //__SCENEMANAGER_H__


