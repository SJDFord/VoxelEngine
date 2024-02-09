#pragma once
#include <glm/glm.hpp>
#include <Texture.h>

// Container for material properties
struct Material {
    Texture* DiffuseMap;
    Texture* SpecularMap;
    Texture* EmissionMap;
    float Shininess;
};