//
// Filename: MeshManager.h
// Created by W. Mysterio on 2022-07-15 03:31:49.
// Description:
// Mail: woden3702@gmail.com
//

#ifndef __MESHMANAGER_H__
#define __MESHMANAGER_H__
#include <memory>
#include <unordered_map>
#include "MeshData.h"

class MeshManager
{
private:
	MeshManager();
	static MeshManager* ins_;

	std::unordered_map<std::string, std::shared_ptr<MeshData>> meshData;
	MeshData buildInBox;
	MeshData buildInSphere;

	void initBuildInBoxMesh();
	void initBuildInSphereMesh();

public:
	~MeshManager();
	static MeshManager* instance();

	std::shared_ptr<MeshData> getMeshData(std::string meshPath, std::string meshName);
	std::shared_ptr<MeshData> CreateNewMeshData(std::string meshPath, std::string meshName);

	bool findMesh(std::string meshPath, std::string meshName);

	std::shared_ptr<MeshData> getBuildInBox() const;
	std::shared_ptr<MeshData> getBuildInSphere() const;
};

#endif //__MESHMANAGER_H__


