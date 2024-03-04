#pragma once

#include <memory>
#include <vector>
#include <engine/Shader.h>
#include <engine/BasicCamera.h>
#include <engine/Transform3.h>
#include <engine/MeshBuffer.h>
#include <engine/Lights.h>

class IMeshRenderer
{
public:
    virtual void render(
        glm::vec2 screenDimensions,
        std::shared_ptr<MeshBuffer> meshBuffer,
        std::shared_ptr<Shader> shader, 
        BasicCamera& camera, 
        Lighting lighting,
        std::vector<Transform3> instanceTransformations
    ) = 0;    // "= 0" part makes this method pure virtual, and
    // also makes this class abstract.
};