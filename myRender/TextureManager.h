//
// Filename: TextureManager.h
// Created by W. Mysterio on 2022-07-15 04:30:27.
// Description:
// Mail: woden3702@gmail.com
//

#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__
#include <unordered_map>
#include <GL/glew.h>

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class TextureManager
{
private:
	TextureManager();
	static TextureManager* ins_;
	std::unordered_map<std::string, GLuint> texturesMap_;
	//两组纹理的位置
	GLuint defaultTex_[3];
	GLuint nullTex_;

public:
	~TextureManager();
	static TextureManager* instance();
	GLuint loadTexture(char* filePath);
	GLuint loadDefaultD() const;
	GLuint loadDefaultN() const;
	GLuint loadDefaultS() const;
	GLuint* loadDefaultTexs();
	GLuint getNullTex() const;
};

#endif //__TEXTUREMANAGER_H__


