//
// Filename: TerrainManager.cpp
// Created by W. Mysterio on 2022-07-24 05:21:13.
// Description:
// Mail: woden3702@gmail.com
//

#include "TerrainManager.h"
#include "TextureManager.h"

#define CR_VERTATTRIPOS_POS 0
#define CR_VERTATTRIPOS_NOR 1
#define CR_VERTATTRIPOS_TAG 2
#define CR_VERTATTRIPOS_UVS 3
#define CR_VERTATTRIPOS_COL 4
#define CR_VERTATTRIPOS_BNL 5
//helper
void setGLArrayBuffer(GLuint bIndex, GLsizeiptr bSize, const GLvoid* bData, GLuint eSize, GLenum eType, GLuint aPos)
{
	glBindBuffer(GL_ARRAY_BUFFER, bIndex);
	glNamedBufferData(bIndex, bSize, bData, GL_STATIC_DRAW);
	glVertexAttribPointer(aPos, eSize, eType, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(aPos);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void* getGLArrayBufferPointer(GLuint bIndex, GLenum access)
{
	return glMapNamedBuffer(bIndex, access);
}

void unmapGLArrayBuffer(GLuint bIndex)
{
	glUnmapNamedBuffer(bIndex);
}

//////////////////////////////////

TerrainManager* TerrainManager::ins_ = nullptr;

TerrainManager* TerrainManager::instance()
{
	if (ins_ == nullptr)
		ins_ = new TerrainManager();

	return ins_;
}

void TerrainManager::createTerrainObject()
{
	if (terrainObj_ == nullptr)
	{
		terrainObj_ = new TerrainObject();
		useTerrain_ = true;
		// useTerrain = false;
	}
}

void TerrainManager::deleteTerrainObject()
{
	delete terrainObj_;
	useTerrain_ = false;
}

TerrainObject* TerrainManager::getTerrainObject()
{
	return terrainObj_;
}

bool TerrainManager::useTerrain()
{
	return useTerrain_;
}


TerrainManager::TerrainManager()
{
	createTerrainObject();
}

TerrainManager::~TerrainManager()
{
	if (useTerrain_)
	{
		delete terrainObj_;
		terrainObj_ = nullptr;
		useTerrain_ = false;
	}

	ins_ = nullptr;
}

void TerrainObject::render(Shader* shader)
{
	updateModelMatrix();

	glm::mat4 finalMat = modelMatrix_;

	glm::mat3 normalMat = finalMat;

	shader->use();
	shader->setMat4("projection", CameraManager::instance()->getCurCamera()->getProjectionMatrix());
	shader->setMat4("view", CameraManager::instance()->getCurCamera()->getViewMatrix());
	shader->setMat4("model", modelMatrix_);

	glActiveTexture(GL_TEXTURE0+0);
	glUniform1i(glGetUniformLocation(shader->ID, "texture_diffuse1"), 0);
	glBindTexture(GL_TEXTURE_2D, TextureManager::instance()->loadDefaultD());

	glActiveTexture(GL_TEXTURE0+1);
	glUniform1i(glGetUniformLocation(shader->ID, "texture_normal1"), 1);
	glBindTexture(GL_TEXTURE_2D, TextureManager::instance()->loadDefaultN());

	glActiveTexture(GL_TEXTURE0+2);
	glUniform1i(glGetUniformLocation(shader->ID, "texture_specular1"), 2);
	glBindTexture(GL_TEXTURE_2D, TextureManager::instance()->loadDefaultS());



	glBindVertexArray(vertexArrayBuffers[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[1]);
	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, NULL);

	for (int i = 0; i < 3; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindVertexArray(NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}

void TerrainObject::update(Camera* cam)
{
	glm::vec3 camPos = cam->getPosition();
	camPos.y = 0.0f;

	float farClip = cam->GetFarClip();
	glm::vec3 intersections[4];
	intersections[0] = glm::vec3(-farClip, 0.0f, -farClip) + camPos;
	intersections[1] = glm::vec3(-farClip, 0.0f, farClip) + camPos;
	intersections[2] = glm::vec3(farClip, 0.0f, -farClip) + camPos;
	intersections[3] = glm::vec3(farClip, 0.0f, farClip) + camPos;

	//update vertex pos
	float* ptrPos = (float*)getGLArrayBufferPointer(vertexArrayBuffers[2], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrPos[i * 3] = intersections[i].x;
		ptrPos[i * 3 + 1] = intersections[i].y;
		ptrPos[i * 3 + 2] = intersections[i].z;
	}

	//update vertex color
	float* ptrColor = (float*)getGLArrayBufferPointer(vertexArrayBuffers[3], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrColor[i * 4] = 1.0f;
		ptrColor[i * 4 + 1] = 1.0f;
		ptrColor[i * 4 + 2] = 1.0f;
		ptrColor[i * 4 + 3] = 1.0f;
	}

	//update vertex uv
	float* ptrUV = (float*)getGLArrayBufferPointer(vertexArrayBuffers[4], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrUV[i * 4] = ptrPos[i] * 0.001f;
		ptrUV[i * 4 + 1] = ptrPos[i * 3 + 2] * 0.001f;
		ptrUV[i * 4 + 2] = 1.0f;
		ptrUV[i * 4 + 3] = 1.0f;
	}

	//update vertex normal
	float* ptrNormal = (float*)getGLArrayBufferPointer(vertexArrayBuffers[5], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrNormal[i * 3] = 0.0f;
		ptrNormal[i * 3 + 1] = 1.0f;
		ptrNormal[i * 3 + 2] = 0.0f;
	}

	//update vertex tangent
	float* ptrTangent = (float*)getGLArrayBufferPointer(vertexArrayBuffers[6], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrTangent[i * 3] = 1.0f;
		ptrTangent[i * 3 + 1] = 0.0f;
		ptrTangent[i * 3 + 2] = 0.0f;
	}

	//update vertex binormal
	float* ptrBinormal = (float*)getGLArrayBufferPointer(vertexArrayBuffers[7], GL_WRITE_ONLY);
	for (int i = 0; i < 4; i++)
	{
		ptrBinormal[i * 3] = 0.0f;
		ptrBinormal[i * 3 + 1] = 0.0f;
		ptrBinormal[i * 3 + 2] = -1.0f;
	}

	for (int i = 2; i < 8; i++)
		unmapGLArrayBuffer(vertexArrayBuffers[i]);
}

// 和初始化meshManager一样,在显存中初始化数据
TerrainObject::TerrainObject()
{
	glGenVertexArrays(1, vertexArrayBuffers);
	glGenBuffers(7, vertexArrayBuffers + 1);

	glBindVertexArray(vertexArrayBuffers[0]);

	unsigned int indexes[6] = {
		0, 1, 2,
		1, 3, 2 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 2 * 3, indexes, GL_STATIC_DRAW);

	float* vertexBuffer = new float[4 * 3];
	float* colorBuffer = new float[4 * 4];
	float* uvBuffer = new float[4 * 4];
	float* normalBuffer = new float[4 * 3];
	float* tangentBuffer = new float[4 * 3];
	float* binormalBuffer = new float[4 * 3];

	setGLArrayBuffer(vertexArrayBuffers[2], sizeof(float) * 4 * 3, vertexBuffer, 3, GL_FLOAT, CR_VERTATTRIPOS_POS);
	setGLArrayBuffer(vertexArrayBuffers[3], sizeof(float) * 4 * 4, colorBuffer, 4, GL_FLOAT, CR_VERTATTRIPOS_COL);
	setGLArrayBuffer(vertexArrayBuffers[4], sizeof(float) * 4 * 4, uvBuffer, 4, GL_FLOAT, CR_VERTATTRIPOS_UVS);
	setGLArrayBuffer(vertexArrayBuffers[5], sizeof(float) * 4 * 3, normalBuffer, 3, GL_FLOAT, CR_VERTATTRIPOS_NOR);
	setGLArrayBuffer(vertexArrayBuffers[6], sizeof(float) * 4 * 3, tangentBuffer, 3, GL_FLOAT, CR_VERTATTRIPOS_TAG);
	setGLArrayBuffer(vertexArrayBuffers[7], sizeof(float) * 4 * 3, binormalBuffer, 3, GL_FLOAT, CR_VERTATTRIPOS_BNL);

	delete[] vertexBuffer;
	delete[] colorBuffer;
	delete[] uvBuffer;
	delete[] normalBuffer;
	delete[] tangentBuffer;
	delete[] binormalBuffer;
}

TerrainObject::~TerrainObject()
{
	glDeleteVertexArrays(1, vertexArrayBuffers);
	glDeleteBuffers(7, vertexArrayBuffers + 1);

	for (int i = 0; i < 8; i++)
		vertexArrayBuffers[i] = -1;
}