#pragma once
#include <LightBuilder.h>

class DirectionalLightBuilder
{

private:
	glm::vec3 _direction;
	LightProperties _properties;
public:
	DirectionalLightBuilder& setDirection(glm::vec3 direction);
	DirectionalLightBuilder& setProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	DirectionalLight build();
};