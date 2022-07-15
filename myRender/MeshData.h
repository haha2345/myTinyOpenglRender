//
// Filename: MeshData.h
// Created by W. Mysterio on 2022-07-15 02:34:25.
// Description:
// Mail: woden3702@gmail.com
//

#ifndef __MESHDATA_H__
#define __MESHDATA_H__
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <gl/glew.h>

#define MAX_BONE_INFLUENCE 4
using uint = unsigned int;

//包含的顶点信息，方便后面地址偏移
struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

class MeshData
{
private:
    GLuint VAO_;
    GLuint VBO_;
    GLuint EBO_;
    uint vertexCount_;
    uint polygonCount_;
    std::vector<uint> indexArray_;
    std::vector<Vertex> vertices_;

    std::string meshPath_;
    std::string meshName_;

    bool bReady4Rendering_;

public:
    MeshData();
    ~MeshData();

    void setVerteces(std::vector<Vertex>);
    void setIndexArray(std::vector<uint>);
    void setMeshName(std::string name);
    void setMeshPath(std::string path);
    void setVertexCount(uint count);
    void setPolygonCount(uint count);

    std::vector<Vertex> getVertices() const;
    std::vector<uint> getIndexArray() const;
    std::string getMeshPath() const;
    std::string getMeshName() const;
    uint getVertexCount() const;
    uint getPolygonCount() const;
    GLuint getVertexArrayObject() const;
    GLuint getElementBufferObject() const;

    //将数据写入到显存中
    void ready4Rendering();

};

#endif //__MESHDATA_H__


