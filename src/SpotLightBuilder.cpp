#include <engine/SpotLightBuilder.h>

SpotLightBuilder& SpotLightBuilder::setPosition(glm::vec3 position)
{
	_position = position;
	return *this;
}

SpotLightBuilder& SpotLightBuilder::setCutOffs(float cutOff, float outerCutOff)
{
	_cutOff = cutOff;
	_outerCutOff = outerCutOff;
	return *this;
}

SpotLightBuilder& SpotLightBuilder::setDirection(glm::vec3 direction)
{
	_direction = direction;
	return *this;
}

SpotLightBuilder& SpotLightBuilder::setProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	_properties = { ambient, diffuse, specular };
	return *this;
}
SpotLightBuilder& SpotLightBuilder::setAttenuation(float constant, float linear, float quadratic)
{
	_attenuation = { constant, linear, quadratic };
	return *this;
}

SpotLight SpotLightBuilder::build()
{
	SpotLight light = { _position, _direction, _cutOff, _outerCutOff, _properties, _attenuation };
	return light;
}