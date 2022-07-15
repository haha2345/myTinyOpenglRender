//
// Filename: Object.h
// Created by W. Mysterio on 2022-07-14 07:50:14.
// Description:
// Mail: woden3702@gmail.com
//

#ifndef __OBJECT_H__
#define __OBJECT_H__
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <string>

enum class ObjectType
{
	oNull,
	oMesh,
	oLight,
	oCamera,
};

class Object
{
protected:
	std::string name_;
	glm::vec3 pos_;
	glm::vec3 scale_;
	glm::vec3 rotate_;
	glm::mat4 modelMatrix_;
	ObjectType objType_;

public:
	Object();
	~Object();

	void setName(std::string& name);
	void setPosition(glm::vec3 pos);
	void setScale(glm::vec3 scale);
	void setRotation(glm::vec3 rotate);
	void updateModelMatrix();

	std::string getName() const;
	glm::vec3 getPosition() const;
	glm::vec3 getScale() const;
	glm::vec3 getRotation() const;
	glm::mat4 getModelMatrix() const;
	ObjectType getObjectType() const;
	
};

#endif //__OBJECT_H__


