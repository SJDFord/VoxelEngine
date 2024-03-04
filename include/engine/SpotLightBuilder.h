
#include <glm/glm.hpp>
#include <engine/Lights.h>

class SpotLightBuilder {
private:
	glm::vec3 _position;
	glm::vec3 _direction;
	float _cutOff;
	float _outerCutOff;
	LightProperties _properties;
	LightAttenuation _attenuation;
public:
	SpotLightBuilder& setPosition(glm::vec3 position);
	SpotLightBuilder& setCutOffs(float cutOff, float outerCutOff);
	SpotLightBuilder& setDirection(glm::vec3 direction);
	SpotLightBuilder& setProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	SpotLightBuilder& setAttenuation(float constant, float linear, float quadratic);
	SpotLight build();
};