//
// Filename: TerrainManager.h
// Created by W. Mysterio on 2022-07-24 05:21:13.
// Description:
// Mail: woden3702@gmail.com
//

#ifndef __TERRAINMANAGER_H__
#define __TERRAINMANAGER_H__
#include "Object.h"
#include "CameraManager.h"

class TerrainObject : public Object
{
public:
	friend class TerrainManager;

	void render(Shader* shader);
	void update(Camera* cam);
private:
	TerrainObject();
	~TerrainObject();

	GLuint vertexArrayBuffers[8];
};

class TerrainManager
{
public:
	~TerrainManager();
	static TerrainManager* instance();

	void createTerrainObject();
	void deleteTerrainObject();
	TerrainObject* getTerrainObject();
	bool useTerrain();
private:
	TerrainManager();

	static TerrainManager* ins_;
	bool useTerrain_;

	TerrainObject* terrainObj_;
};

#endif //__TERRAINMANAGER_H__


