#pragma once
#include <glm/glm.hpp>

struct Vertex {
    // Local space coords
    glm::vec3 Position;

    // Local normal coords
    glm::vec3 Normal;

    // Texture coordinates
    glm::vec2 TexCoords;
};