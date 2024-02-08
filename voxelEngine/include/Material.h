#pragma once
#include <glm/glm.hpp>

// Container material properties
struct Material {
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    float Shininess;
};

struct MaterialPreset {

    static constexpr Material Test = { {1.0f, 0.5f, 0.31f}, {1.0f, 0.5f, 0.31f}, {0.5f, 0.5f, 0.5f}, 32 };
    static constexpr Material Emerald = { {0.0215, 0.1745, 0.0215},	{0.07568, 0.61424, 0.07568}, {0.633,0.727811,0.633}, 0.6f };
};

/*

lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
lightingShader.setFloat("material.shininess", 32.0f);
*/