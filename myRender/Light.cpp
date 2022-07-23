//
// Filename: Light.cpp
// Created by W. Mysterio on 2022-07-23 03:35:08.
// Description:
// Mail: woden3702@gmail.com
//

#include "Light.h"

Light::Light(LightType type, float intensity)
{
	objType_ = ObjectType::oLight;
	lightType_ = type;
	name_ = "light";
	setIntensity(intensity);
}

Light::~Light()
{
}

LightType Light::getType() const
{
	return lightType_;
}

float Light::getIntensity() const
{
	return intensity_;
}

glm::vec3 Light::getColor() const
{
	return {color_};
}

void Light::setColor(glm::vec3 color)
{
	color_ = color;
}

void Light::setLightType(LightType type)
{
	lightType_ = type;
}

void Light::setIntensity(float intensity)
{
	intensity_ = intensity;
}
