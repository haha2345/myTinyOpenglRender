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

#include "Object.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "Shader.h"

using uint = unsigned int;

class MeshObject : public Object
{
private:
	uint textureId_;
	std::shared_ptr<MeshData> meshData_;

public:
	MeshObject();
	~MeshObject();

	void SetTexture(unsigned int id);
	void SetMeshData(std::shared_ptr<MeshData> data);

	uint GetTexture();
	std::shared_ptr<MeshData> GetMeshData();

	void Render(Shader* shader, bool useTex = true);
	
};

#endif //__MESHOBJECT_H__


