//
// Filename: Light.h
// Created by W. Mysterio on 2022-07-23 03:35:08.
// Description:
// Mail: woden3702@gmail.com
//

#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "Object.h"

enum class LightType
{
	Direction = 0,
	Point = 1,
	Spot = 2,
};

class Light:public Object
{
private:
	LightType lightType_;
	float intensity_;
	glm::vec3 color_;

public:
	Light(LightType type, float intensity);
	~Light();

	LightType getType() const;
	float getIntensity() const;
	glm::vec3 getColor() const;

	void setColor(glm::vec3 color);
	void setLightType(LightType type);
	void setIntensity(float intensity);
};

#endif //__LIGHT_H__


