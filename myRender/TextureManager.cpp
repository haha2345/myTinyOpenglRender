//
// Filename: TextureManager.cpp
// Created by W. Mysterio on 2022-07-15 04:30:27.
// Description:
// Mail: woden3702@gmail.com
//

#include "TextureManager.h"

TextureManager* TextureManager::ins_ = nullptr;


TextureManager::TextureManager()
{
	GLubyte D[8][8][3];
	GLubyte N[8][8][3];
	GLubyte S[8][8][3];
	GLubyte nullTexData[8][8][4];
	int i, j;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			D[i][j][0] = (GLubyte)255;
			D[i][j][1] = (GLubyte)255;
			D[i][j][2] = (GLubyte)255;
		}
	}

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			N[i][j][0] = (GLubyte)128;
			N[i][j][1] = (GLubyte)128;
			N[i][j][2] = (GLubyte)255;
		}
	}

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			S[i][j][0] = (GLubyte)255;//Roughness
			S[i][j][1] = (GLubyte)255;//Metallic
			S[i][j][2] = (GLubyte)0;
		}
	}

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			nullTexData[i][j][0] = (GLubyte)0;
			nullTexData[i][j][1] = (GLubyte)0;
			nullTexData[i][j][2] = (GLubyte)0;
			nullTexData[i][j][3] = (GLubyte)0;
		}
	}

	//生成纹理名称 n-纹理数量 texture-指定存储生成的纹理ID的数组
	glGenTextures(3, defaultTex_);
	//将一个命名的纹理<texture>绑定到一个纹理目标<target>上
	glBindTexture(GL_TEXTURE_2D, defaultTex_[0]);
	//纹理过滤函数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 8, 8, 0, GL_RGB, GL_UNSIGNED_BYTE, D);

	/*glTexImage2D(GL_TEXTURE_2D,　　      //此纹理是一个2D纹理
		0,　　　　　　　　　　//代表图像的详细程度, 默认为0即可
		3,　　　　　　　　　　//颜色成分R(红色分量)、G(绿色分量)、B(蓝色分量)三部分，若为4则是R(红色分量)、G(绿色分量)、B(蓝色分量)、Alpha
		TextureImage[0]->sizeX,　　//纹理的宽度
		TextureImage[0]->sizeY,　　//纹理的高度
		0,　　　　　　　　　　      //边框的值
		GL_RGB,　　　　　　        //告诉OpenGL图像数据由红、绿、蓝三色数据组成
		GL_UNSIGNED_BYTE,　     //组成图像的数据是无符号字节类型
		TextureImage[0]->data);　　//告诉OpenGL纹理数据的来源,此例中指向存放在TextureImage[0]记录中的数据
	使用glTexImage2D()时所采用的位图文件分辨率必须为：64×64、128×128、256×256三种格式，如果其他大小则会出现绘制不正常。*/

	glBindTexture(GL_TEXTURE_2D, defaultTex_[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 8, 8, 0, GL_RGB, GL_UNSIGNED_BYTE, N);

	glBindTexture(GL_TEXTURE_2D, defaultTex_[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 8, 8, 0, GL_RGB, GL_UNSIGNED_BYTE, S);

	glGenTextures(1, &nullTex_);//第二个参数使用引用，上面因为使用数组作为变量指的就是数组中第一元素的引用
	glBindTexture(GL_TEXTURE_2D, nullTex_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullTexData);
}

TextureManager::~TextureManager()
{
	glDeleteTextures(3, defaultTex_);

	for (auto i = texturesMap_.begin(); i != texturesMap_.end(); i++)
		glDeleteTextures(1, &(i->second));

	texturesMap_.clear();
	ins_ = nullptr;
}

TextureManager* TextureManager::instance()
{
	if (ins_ == nullptr)
		ins_ = new TextureManager();
	return ins_;
}

GLuint TextureManager::loadTexture(char* filePath)
{
	return 0;
}

GLuint TextureManager::loadDefaultD() const
{
	return defaultTex_[0];
}

GLuint TextureManager::loadDefaultN() const
{
	return defaultTex_[1];
}

GLuint TextureManager::loadDefaultS() const
{
	return defaultTex_[2];
}

GLuint* TextureManager::loadDefaultTexs() 
{
	return defaultTex_;
}

GLuint TextureManager::getNullTex() const
{
	return nullTex_;
}
