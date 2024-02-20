#include <LightBuilder.h>


LightBuilder& LightBuilder::setProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	_properties = { ambient, diffuse, specular };
	return *this;
}