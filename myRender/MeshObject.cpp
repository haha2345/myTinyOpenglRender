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

void MeshObject::setTexture(unsigned id)
{
	textureId_ = id;
}

void MeshObject::setMeshData(std::shared_ptr<MeshData> data)
{
	meshData_ = data;
}

void MeshObject::setTexture(std::vector<std::vector<Texture>> textures)
{
	textures_ = textures;
}

void MeshObject::setMeshData(std::vector<std::shared_ptr<MeshData>> meshes)
{
	meshDatas_ = meshes;
}

void MeshObject::setMeshType(MeshType type)
{
	meshType_ = type;
}

uint MeshObject::getTexture()
{
	return textureId_;
}

std::shared_ptr<MeshData> MeshObject::getMeshData()
{
	return meshData_;
}

void MeshObject::renderBuildInModel(Shader* shader, bool useTex)
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

void MeshObject::renderLoadedModel(Shader* shader)
{
	for(int i=0;i<meshDatas_.size();++i)
	{
		if (meshDatas_[i] == nullptr)
			return;

		meshDatas_[i]->ready4Rendering();
		// printf("第%d次循环，纹理个数：%d\n", i,textures_[i].size());

		updateModelMatrix();


		shader->use();
		shader->setMat4("projection", CameraManager::instance()->getCurCamera()->getProjectionMatrix());
		shader->setMat4("view", CameraManager::instance()->getCurCamera()->getViewMatrix());
		shader->setMat4("model", modelMatrix_);

		// bind appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int j = 0; j < textures_[i].size(); j++)
		{
			glActiveTexture(GL_TEXTURE0 + j); // active proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = textures_[i][j].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to string
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to string
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to string

			// now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), j);
			// and finally bind the texture
			glBindTexture(GL_TEXTURE_2D, textures_[i][j].id);
		}



		glBindVertexArray(meshDatas_[i]->getVertexArrayObject());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshDatas_[i]->getElementBufferObject());
		// glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(meshData_->getIndexArray().size()), GL_UNSIGNED_INT, 0);

		glDrawElements(GL_TRIANGLES, meshDatas_[i]->getPolygonCount() * 3, GL_UNSIGNED_INT, NULL);

		glBindVertexArray(NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

		// always good practice to set everything back to defaults once configured.
		glActiveTexture(GL_TEXTURE0);
	}
}

void MeshObject::render(Shader* shader)
{
	if (meshType_==MeshType::buildIn)
	{
		renderBuildInModel(shader);
	}
	else if (meshType_ == MeshType::loaded)
	{
		renderLoadedModel(shader);
	}
}
