//
// Filename: Camera.h
// Created by W. Mysterio on 2022-07-04 03:22:45.
// Description:
// Mail: woden3702@gmail.com
//

#ifndef __CAMERA_H__
#define __CAMERA_H__
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp>
#include "WindowManager.h"

class Camera
{
private:
	glm::vec3 position_; // 相机位置
	glm::vec3 cameraTarget_; //相机朝向
	glm::vec3 cameraDirection_; //相机的方向
	glm::vec3 cameraUp_; //相机的上方
	glm::vec3 cameraRight_; //相机的右轴

	float fov_; //度
	float farClip_;
	float nearClip_;

	float inWidth_;
	float inHeight_;
	float aspect_;
	glm::mat4 projectionMatrix_;
	glm::mat4 viewMatrix_;
	glm::mat4 modelMatrix_;
	glm::mat4 modelViewMatrix_;


public:

	Camera();
	~Camera();
	void SetPerspectiveCamera(float iFov, float iNearClip, float iFarClip);	void SetNearClip(float iNearClip);
	void setPosition(glm::vec3 ipos);
	void setDirection(glm::vec3 idir);
	float GetNearClip() const;
	void SetFarClip(float iFarClip);
	float GetFarClip() const;
	void SetFov(float iFov);
	float GetFov() const;
	void updateProjectionMatrix();
	void updateViewMatrix();
	void updateModelMatrix();
	void updateModelViewMatrix();
	glm::mat4 getModelViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;
	glm::mat4 getViewMatrix() const;
	glm::vec3 getPosition() const;
	glm::vec3 getRight() const;
	glm::vec3 getDirection() const;
	glm::vec3 getUp() const;
};

#endif //__CAMERA_H__


