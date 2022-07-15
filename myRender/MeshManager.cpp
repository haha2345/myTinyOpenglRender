//
// Filename: MeshManager.cpp
// Created by W. Mysterio on 2022-07-15 03:31:49.
// Description:
// Mail: woden3702@gmail.com
//

#include "MeshManager.h"

MeshManager* MeshManager::ins_ = nullptr;

MeshManager::MeshManager()
{
	initBuildInBoxMesh();
	// initBuildInSphereMesh();
}

void MeshManager::initBuildInBoxMesh()
{
	std::vector<Vertex> vertices;
	std::vector<uint> indexArray;


	float vertexPos[72] = {
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f
	};

	//24
	float TexCoords[48] = {
		1.0f, 1.0f,
		0.0f, 1.0f, 
		1.0f, 0.0f, 
		0.0f, 1.0f, 
		1.0f, 0.0f, 
		0.0f, 0.0f, 
		1.0f, 0.0f, 
		0.0f, 1.0f, 
		1.0f, 1.0f, 
		1.0f, 1.0f,
		1.0f, 1.0f, 
		0.0f, 1.0f, 
		0.0f, 1.0f, 
		0.0f, 1.0f, 
		1.0f, 0.0f, 
		1.0f, 1.0f, 
		0.0f, 0.0f, 
		1.0f, 0.0f, 
		0.0f, 0.0f,
		0.0f, 0.0f, 
		0.0f, 0.0f, 
		0.0f, 0.0f,
	};

	float vertexNormal[72] = {
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 1.0f
	};

	float vertexTangent[72] = {
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	float vertexBinormal[72] = {
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 0.0f,
		-0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		-0.0f, -0.0f, -1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	unsigned int index[36] = {
	1, 2, 0,
	3, 6, 8,
	7, 4, 9,
	5, 10, 11,
	12, 13, 14,
	15, 16, 17,
	1, 18, 2,
	3, 19, 6,
	7, 20, 4,
	5, 21, 10,
	12, 22, 13,
	15, 23, 16
	};

	for(uint i=0;i<24;i++)
	{
		Vertex vertex;
		glm::vec3 vec3;
		//顶点位置
		vec3.x = vertexPos[i * 3];
		vec3.y = vertexPos[i * 3 + 1];
		vec3.z = vertexPos[i * 3 + 2];
		vertex.Position = vec3;
		//顶点法线
		vec3.x = vertexNormal[i * 3];
		vec3.y = vertexNormal[i * 3 + 1];
		vec3.z = vertexNormal[i * 3 + 2];
		vertex.Normal = vec3;
		//顶点Tangent
		vec3.x = vertexTangent[i * 3];
		vec3.y = vertexTangent[i * 3 + 1];
		vec3.z = vertexTangent[i * 3 + 2];
		vertex.Tangent = vec3;
		//顶点Binormal
		vec3.x = vertexBinormal[i * 3];
		vec3.y = vertexBinormal[i * 3 + 1];
		vec3.z = vertexBinormal[i * 3 + 2];
		vertex.Bitangent = vec3;
		//顶点纹理
		glm::vec2 vec2;
		vec2.x = TexCoords[i * 2];
		vec2.y = TexCoords[i * 2 + 1];
		vertex.TexCoords = vec2;

		vertices.push_back(vertex);
	}

	for (auto a:index)
	{
		indexArray.push_back(a);
	}

	buildInBox.setVertexCount(24);
	buildInBox.setVerteces(vertices);
	buildInBox.setIndexArray(indexArray);
	buildInBox.setMeshName("box");
	buildInBox.setMeshPath("./");
	buildInBox.setPolygonCount(12);
	buildInBox.ready4Rendering();
}

void MeshManager::initBuildInSphereMesh()
{
}

MeshManager::~MeshManager()
{
}

MeshManager* MeshManager::instance()
{
	if (ins_ == nullptr)
		ins_ = new MeshManager();
	return ins_;
}

std::shared_ptr<MeshData> MeshManager::getMeshData(std::string meshPath, std::string meshName)
{
	//TODO
	return std::make_shared<MeshData>(buildInBox);
}

std::shared_ptr<MeshData> MeshManager::CreateNewMeshData(std::string meshPath, std::string meshName)
{
	//TODO
	return std::make_shared<MeshData>(buildInBox);
}

bool MeshManager::findMesh(std::string meshPath, std::string meshName)
{
	return false;
}

std::shared_ptr<MeshData> MeshManager::getBuildInBox() const
{
	return std::make_shared<MeshData>(buildInBox);
}

std::shared_ptr<MeshData> MeshManager::getBuildInSphere() const
{
	return std::make_shared<MeshData>(buildInSphere);
}
