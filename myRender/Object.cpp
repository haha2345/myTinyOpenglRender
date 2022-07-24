//
// Filename: Object.cpp
// Created by W. Mysterio on 2022-07-14 07:50:14.
// Description:
// Mail: woden3702@gmail.com
//

#include "Object.h"

Object::Object()
{
	name_ = "Object";
	pos_ = glm::vec3(0.0f,0.0f,0.0f);
	scale_ = glm::vec3(1.0f);
	rotate_ = glm::vec3(1.0f, 1.0f, 1.0f);
	objType_ = ObjectType::oNull;
}

Object::~Object() = default;

void Object::setName(std::string name)
{
	name_ = name;
}

void Object::setPosition(glm::vec3 pos)
{
	pos_ = pos;
}

void Object::setScale(glm::vec3 scale)
{
	scale_ = scale;
}

void Object::setRotation(glm::vec3 rotate)
{
	rotate_ = rotate;
}

void Object::updateModelMatrix()
{
	modelMatrix_ = glm::translate(glm::mat4(1.0f), pos_);
	modelMatrix_ = glm::scale(modelMatrix_, scale_);
	modelMatrix_ = glm::rotate(modelMatrix_, 0.0f, rotate_);
}

std::string Object::getName() const
{
	return name_;
}

glm::vec3 Object::getPosition() const
{
	return {pos_};
}

glm::vec3 Object::getScale() const
{
	return {scale_};
}

glm::vec3 Object::getRotation() const
{
	return {rotate_};
}

glm::mat4 Object::getModelMatrix() const
{
	return {modelMatrix_};
}

ObjectType Object::getObjectType() const
{
	return objType_;
}
