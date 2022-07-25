//
// Filename: SceneManager.cpp
// Created by W. Mysterio on 2022-07-24 09:38:26.
// Description:
// Mail: woden3702@gmail.com
//

#include "SceneManager.h"

SceneManager* SceneManager::ins_ = nullptr;
SceneManager::SceneManager()
{
	sceneName_ = "new scene";
	init();
}

SceneManager::~SceneManager()
{
	ins_ = nullptr;
}

SceneManager* SceneManager::instance()
{
	if (ins_ == nullptr)
		ins_ = new SceneManager();
	return ins_;
}

void SceneManager::init()
{
	auto mo = new MeshObject();
	mo->setMeshData(MeshManager::instance()->getBuildInBox());
	mo->setName("cube");
	mo->setTexture(TextureManager::instance()->loadDefaultD());
	mo->setMeshType(MeshType::buildIn);
	mo->setPosition(glm::vec3(1.0, 0.0, 1.0));
	mo->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	objects_.push_back(std::shared_ptr<Object>(mo));

	// auto mo1 = new MeshObject();
	// mo1->setMeshData(MeshManager::instance()->getBuildInBox());
	// mo1->setName("plane");
	// mo1->setTexture(TextureManager::instance()->loadDefaultD());
	// mo1->setMeshType(MeshType::buildIn);
	// mo1->setPosition(glm::vec3(1.0, 0.0, 1.0));
	// mo1->setScale(glm::vec3(10.0f, 0.1f, 10.0f));
	// objects_.push_back(std::shared_ptr<Object>(mo1));

}

void SceneManager::draw(Shader* shader)
{
	for (const auto& object : objects_)
	{
		object->render(shader);
	}
}

void SceneManager::deleteObject(std::shared_ptr<Object> obj)
{
}

void SceneManager::addObject(std::shared_ptr<Object> obj)
{
	printf("管理器：%p\n", obj);
	objects_.push_back(obj);
}


inline void SceneManager::setSceneName(std::string name)
{
	sceneName_ = name;
}

inline std::string SceneManager::getSceneName() const
{
	return  sceneName_ ;
}

std::list<std::shared_ptr<Object>> SceneManager::getObjects() const
{
	return objects_;
}
