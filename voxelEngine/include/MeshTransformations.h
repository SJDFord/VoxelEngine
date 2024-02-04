#pragma once
#include <glm/glm.hpp>

// Container for instance-specific transformations of the same mesh
struct MeshTransformations {
    // Scale in x, y, z by this factor
    glm::vec3 ScaleFactor;

    // Rotate by this angle in radians
    float RotationAngle;

    // Rotate around this axis
    glm::vec3 RotationAxis; 

    // Translate to this position
    glm::vec3 Position;
};