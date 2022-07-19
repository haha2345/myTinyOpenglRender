//
// Filename: FileImportManager.cpp
// Created by W. Mysterio on 2022-07-17 02:24:26.
// Description:
// Mail: woden3702@gmail.com
//

#include "FileImportManager.h"



unsigned int TextureFromFile(const char* path, const std::string& directory)
{
	std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

FileImportManager* FileImportManager::ins_ = nullptr;

FileImportManager::FileImportManager()
{
    loadedModel_ = std::make_shared<MeshObject>();
}

void FileImportManager::loadFile(std::string path)
{
    const aiScene* scene = importer_.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer_.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    std::string directory = path.substr(0, path.find_last_of('/'));
    printf(directory.c_str());
    printf("\n");

    path_ = directory;

    std::vector<Vertex> vertices;

    for(int i=0;i<scene->mNumMeshes;++i)
    {
        meshVector_.emplace_back(std::make_shared<MeshData>());
        auto mesh = meshVector_.back();

        aiMesh* aimesh = scene->mMeshes[i];

        // std::vector<Vertex> vertices;

        vertices.clear();

        //读取mesh中的顶点信息
        for(int j=0;j<aimesh->mNumVertices;++j)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = aimesh->mVertices[j].x;
            vector.y = aimesh->mVertices[j].y;
            vector.z = aimesh->mVertices[j].z;
            vertex.Position = vector;
            // normals
            if (aimesh->HasNormals())
            {
                vector.x = aimesh->mNormals[j].x;
                vector.y = aimesh->mNormals[j].y;
                vector.z = aimesh->mNormals[j].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if (aimesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = aimesh->mTextureCoords[0][j].x;
                vec.y = aimesh->mTextureCoords[0][j].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = aimesh->mTangents[j].x;
                vector.y = aimesh->mTangents[j].y;
                vector.z = aimesh->mTangents[j].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = aimesh->mBitangents[j].x;
                vector.y = aimesh->mBitangents[j].y;
                vector.z = aimesh->mBitangents[j].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        mesh->setVertexCount(aimesh->mNumVertices);
        mesh->setVerteces(vertices);
        mesh->setPolygonCount(aimesh->mNumFaces);

        ////////////读取索引信息/////////////

        std::vector<uint> indices;
        
        for (unsigned int i = 0; i < aimesh->mNumFaces; i++)
        {
            aiFace face = aimesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        mesh->setIndexArray(indices);

        //////////////读取材质信息////////////////

        if(aimesh->mMaterialIndex>=0)
        {
            aiMaterial* material = scene->mMaterials[aimesh->mMaterialIndex];

            std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures_.insert(textures_.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. specular maps
            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures_.insert(textures_.end(), specularMaps.begin(), specularMaps.end());
            // 3. normal maps
            std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
            textures_.insert(textures_.end(), normalMaps.begin(), normalMaps.end());
            // 4. height maps
            std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
            textures_.insert(textures_.end(), heightMaps.begin(), heightMaps.end());
        }
    }
    loadedTextures_.push_back(textures_);

    loadedModel_->setMeshData(meshVector_);
    loadedModel_->setTexture(loadedTextures_);
}

std::vector<Texture> FileImportManager::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures_.size(); j++)
        {
            if (std::strcmp(textures_[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), path_);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}


FileImportManager::~FileImportManager()
{
}

FileImportManager* FileImportManager::instance()
{
	if (ins_ == nullptr)
		ins_ = new FileImportManager();
	return ins_;
}

std::shared_ptr<MeshData> FileImportManager::readMeshData(std::string path)
{
    if (loadedData.find(path) == loadedData.end())
    {
        loadFile(path);
    }
    auto data = loadedData[path];
    return nullptr;
}
