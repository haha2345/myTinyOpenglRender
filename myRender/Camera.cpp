//
// Filename: Camera.cpp
// Created by W. Mysterio on 2022-07-04 03:22:45.
// Description:
// Mail: woden3702@gmail.com
//

#include "Camera.h"
float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }
glm::vec3 up= glm::vec3(0.0, 1.0f, 0.0f);
glm::vec3 front = glm::vec3(0.0, 0.0f, -1.0f);

Camera::Camera()
{
	position_ = glm::vec3(0.0f, 1.0f, 3.0f);
	cameraTarget_ = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraDirection_ = glm::vec3(0.0, 0.0f, -1.0f);
	cameraUp_ = glm::vec3(0.0, 1.0f, 0.0f);
	cameraRight_ = glm::normalize(glm::cross(cameraDirection_, cameraUp_));

	fov_ = 60.0f; //度
	farClip_ = 1000.0f;
	nearClip_ = 0.1f;
	inWidth_ = WindowManager::instance()->getWindowSize().w;
	inHeight_ = WindowManager::instance()->getWindowSize().h;
	aspect_ = inWidth_ / inHeight_;
	updateProjectionMatrix();
	updateViewMatrix();
}

Camera::~Camera() = default;

void Camera::updateProjectionMatrix()
{
	projectionMatrix_= glm::perspective(toRadians(fov_), aspect_, nearClip_, farClip_);
}

void Camera::SetNearClip(float iNearClip)
{
	nearClip_ = iNearClip;
}

void Camera::setPosition(glm::vec3 ipos)
{
	position_ = ipos;
}

void Camera::setDirection(glm::vec3 idir)
{
	cameraDirection_ = idir;
}

float Camera::GetNearClip() const
{
	return nearClip_;
}

void Camera::SetFarClip(float iFarClip)
{
	farClip_ = iFarClip;
}

float Camera::GetFarClip() const
{
	return farClip_;
}

void Camera::SetFov(float iFov)
{
	fov_ = iFov;
}

float Camera::GetFov() const
{
	return fov_;
}

void Camera::updateViewMatrix()
{

	viewMatrix_= glm::lookAt(position_,position_+cameraDirection_,cameraUp_);
}

void Camera::updateModelMatrix()
{
	modelMatrix_ = glm::translate(glm::mat4(1.0f),cameraTarget_);
}

void Camera::updateModelViewMatrix()
{
	modelViewMatrix_ = viewMatrix_ * modelMatrix_;
}

glm::mat4 Camera::getModelViewMatrix() const
{
	return modelViewMatrix_;
}

glm::mat4 Camera::getProjectionMatrix() const
{
	return projectionMatrix_;
}

glm::mat4 Camera::getViewMatrix() const
{
	return viewMatrix_;
}

glm::vec3 Camera::getPosition() const
{
	return position_;
}

glm::vec3 Camera::getRight() const
{
	return glm::normalize(glm::cross(cameraDirection_, cameraUp_));
}

glm::vec3 Camera::getDirection() const
{
	return cameraDirection_;
}

glm::vec3 Camera::getUp() const
{
	return cameraUp_;
}
