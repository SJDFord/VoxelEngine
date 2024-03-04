#pragma once
#include <glm/glm.hpp>
#include <vector>

struct LightProperties {
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
};

struct LightAttenuation {
    float Constant;
    float Linear;
    float Quadratic;
};

struct DirectionalLight {
    glm::vec3 Direction;
    LightProperties Properties;
};

struct PointLight {
    glm::vec3 Position;
    LightProperties Properties;
    LightAttenuation Attenuation;
};

struct SpotLight {
    glm::vec3 Position;
    glm::vec3 Direction;
    float CutOff;
    float OuterCutOff;

    LightProperties Properties;
    LightAttenuation Attenuation;
};

struct Lighting {
    std::vector<DirectionalLight> DirectionalLights;
    std::vector<PointLight> PointLights;
    std::vector<SpotLight> SpotLights;
};