//
// Filename: MeshData.cpp
// Created by W. Mysterio on 2022-07-15 02:34:25.
// Description:
// Mail: woden3702@gmail.com
//

#include "MeshData.h"

#include <GL/glew.h>

MeshData::MeshData()
{
	bReady4Rendering_ = false;
	meshName_ = "";
	meshPath_ = "";
}

MeshData::~MeshData()
{
    if (bReady4Rendering_)
    {
        glDeleteVertexArrays(1, &VAO_);
        glDeleteBuffers(1, &VBO_);
        glDeleteBuffers(1, &EBO_);
    }
}

void MeshData::setVerteces(std::vector<Vertex> vertices)
{
	vertices_ = vertices;
}

void MeshData::setIndexArray(std::vector<uint> index)
{
	indexArray_ = index;
}

void MeshData::setMeshName(std::string name)
{
	meshName_ = name;
}

void MeshData::setMeshPath(std::string path)
{
	meshPath_ = path;
}

void MeshData::setVertexCount(uint count)
{
	vertexCount_ = count;
}

void MeshData::setPolygonCount(uint count)
{
	polygonCount_ = count;
}

std::vector<Vertex> MeshData::getVertices() const
{
	return vertices_;
}

std::vector<uint> MeshData::getIndexArray() const
{
	return indexArray_;
}

std::string MeshData::getMeshPath() const
{
	return {meshPath_};
}

std::string MeshData::getMeshName() const
{
	return {meshName_};
}

uint MeshData::getVertexCount() const
{
	return vertexCount_;
}

uint MeshData::getPolygonCount() const
{
	return polygonCount_;
}

GLuint MeshData::getVertexArrayObject() const
{
	return VAO_;
}

GLuint MeshData::getElementBufferObject() const
{
	return EBO_;
}


void MeshData::ready4Rendering()
{
    //避免重复加载数据
    if(bReady4Rendering_)
        return;

    // create buffers/arrays
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * polygonCount_ * 3, &indexArray_[0], GL_STATIC_DRAW);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices_.size() * sizeof(unsigned int), &vertices_[0], GL_STATIC_DRAW);


    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    // ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

    // weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    bReady4Rendering_ = true;
}
