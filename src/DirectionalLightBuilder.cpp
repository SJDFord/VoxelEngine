#include <engine/DirectionalLightBuilder.h>


DirectionalLightBuilder& DirectionalLightBuilder::setDirection(glm::vec3 direction)
{
	_direction = direction;
	return *this;
}

DirectionalLightBuilder& DirectionalLightBuilder::setProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	_properties = { ambient, diffuse, specular };
	return *this;
}

DirectionalLight DirectionalLightBuilder::build()
{
	DirectionalLight light = { _direction, _properties };
	return light;
}