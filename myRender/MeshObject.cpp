//
// Filename: MeshObject.cpp
// Created by W. Mysterio on 2022-07-15 02:09:20.
// Description:
// Mail: woden3702@gmail.com
//

#include "MeshObject.h"

MeshObject::MeshObject()
{
	Object();
	objType_ = ObjectType::oMesh;
	textureId_ = 0;
	meshData_ = nullptr;
}

MeshObject::~MeshObject()
{
}

void MeshObject::SetTexture(unsigned id)
{
	textureId_ = id;
}

void MeshObject::SetMeshData(std::shared_ptr<MeshData> data)
{
	meshData_ = data;
}

uint MeshObject::GetTexture()
{
	return textureId_;
}

std::shared_ptr<MeshData> MeshObject::GetMeshData()
{
	return meshData_;
}

void MeshObject::Render(Shader* shader, bool useTex)
{
	if (meshData_ == nullptr)
		return;

	// printf("%d",textureId_);
	// printf("\n");

	updateModelMatrix();


	if (useTex)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureManager::Instance()->LoadDefaultD());

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TextureManager::Instance()->LoadDefaultN());

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, TextureManager::Instance()->LoadDefaultS());
	}

	shader->use();
	shader->setMat4("projection", CameraManager::instance()->getCurCamera()->getProjectionMatrix());
	shader->setMat4("view", CameraManager::instance()->getCurCamera()->getViewMatrix());
	shader->setMat4("model", modelMatrix_);

	glBindVertexArray(meshData_->getVertexArrayObject());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData_->getElementBufferObject());
	// glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(meshData_->getIndexArray().size()), GL_UNSIGNED_INT, 0);

	glDrawElements(GL_TRIANGLES, meshData_->getPolygonCount() * 3, GL_UNSIGNED_INT, NULL);

	if (useTex)
	{
		for (int i = 0; i < 3; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	glBindVertexArray(NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}
