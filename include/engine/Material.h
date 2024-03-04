#pragma once
#include <glm/glm.hpp>
#include <engine/Texture.h>
#include <memory>

// Container for material properties
struct Material {
    std::shared_ptr<Texture> DiffuseMap;
    std::shared_ptr<Texture> SpecularMap;
    std::shared_ptr<Texture> EmissionMap;
    float Shininess;
};