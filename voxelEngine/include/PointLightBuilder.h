#pragma once

#include <glm/glm.hpp>
#include <Lights.h>

class PointLightBuilder
{
private:
	glm::vec3 _position;
	LightProperties _properties;
	LightAttenuation _attenuation;
public:
	PointLightBuilder& setPosition(glm::vec3 direction);
	PointLightBuilder& setProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	PointLightBuilder& setAttenuation(float constant, float linear, float quadratic);
	PointLight build();
};