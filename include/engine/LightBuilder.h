#pragma once
#include <engine/Lights.h>

class LightBuilder
{
protected:
	LightProperties _properties;

public:
	LightBuilder& setProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
};