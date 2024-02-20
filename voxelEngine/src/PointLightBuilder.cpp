#include <PointLightBuilder.h>

PointLightBuilder& PointLightBuilder::setPosition(glm::vec3 position)
{
	_position = position;
	return *this;
}
PointLightBuilder& PointLightBuilder::setProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	_properties = { ambient, diffuse, specular };
	return *this;
}


PointLightBuilder& PointLightBuilder::setAttenuation(float constant, float linear, float quadratic)
{
	_attenuation = { constant, linear, quadratic };
	return *this;
}

PointLight PointLightBuilder::build()
{
	PointLight light = { _position, _properties, _attenuation };
	return light;
}