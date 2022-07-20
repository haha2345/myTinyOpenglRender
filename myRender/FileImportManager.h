//
// Filename: FileImportManager.h
// Created by W. Mysterio on 2022-07-17 02:24:26.
// Description:
// Mail: woden3702@gmail.com
//

#ifndef __FILEIMPORTMANAGER_H__
#define __FILEIMPORTMANAGER_H__

#include <memory>
#include <iostream>
#include <string>
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SOIL2/stb_image.h>
#include "TextureManager.h"
#include "MeshManager.h"
#include "MeshObject.h"

struct FileData
{
	std::vector<std::shared_ptr<MeshData>> meshData;
	std::vector<std::shared_ptr<Texture>> textures;
};

class FileImportManager
{
private:
	static FileImportManager* ins_;
	FileImportManager();

	std::string path_;

	Assimp::Importer importer_;

	std::unordered_map<std::string, std::shared_ptr<MeshObject>> models_; //保存的模型

	std::vector<std::vector<Texture>> loadedTextures_; //传递给mesObject
	std::vector<std::shared_ptr<MeshData>> meshVector_; //传递给meshObject

	std::shared_ptr<MeshObject> loadedModel_;

	std::vector<Texture> textures_;

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
	~FileImportManager();
	static FileImportManager* instance();
	std::shared_ptr<MeshData> readMeshData(std::string path);
	void loadFile(std::string path);

	void show(Shader* shader)
	{
		loadedModel_->render(shader);
	}
};

#endif //__FILEIMPORTMANAGER_H__


