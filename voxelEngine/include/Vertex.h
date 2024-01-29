#pragma once
#include <glm/glm.hpp>

struct Vertex {
    // Local space coords
    glm::vec3 Position;

    // Texture coordinates
    glm::vec2 TexCoords;
};