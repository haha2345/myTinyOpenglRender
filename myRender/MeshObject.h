//
// Filename: MeshObject.h
// Created by W. Mysterio on 2022-07-15 02:09:20.
// Description:
// Mail: woden3702@gmail.com
//

#ifndef __MESHOBJECT_H__
#define __MESHOBJECT_H__
#include <unordered_map>
#include <memory>
#include <vector>

#include "Object.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "Shader.h"

using uint = unsigned int;

enum class MeshType
{
	buildIn,
	loaded,
};

class MeshObject : public Object
{
private:
	uint textureId_;
	std::shared_ptr<MeshData> meshData_;

	std::vector<std::vector<Texture>> textures_;
	std::vector<std::shared_ptr<MeshData>> meshDatas_;

	MeshType meshType_;

public:
	MeshObject();
	~MeshObject();

	void setTexture(unsigned int id);
	void setMeshData(std::shared_ptr<MeshData> data);

	void setTexture(std::vector<std::vector<Texture>> textures);
	void setMeshData(std::vector<std::shared_ptr<MeshData>> meshes);

	void setMeshType(MeshType type);

	uint getTexture();
	std::shared_ptr<MeshData> getMeshData();

	void renderBuildInModel(Shader* shader, bool useTex = true);
	void renderLoadedModel(Shader* shader);

	void render(Shader* shader) override;
};

#endif //__MESHOBJECT_H__


