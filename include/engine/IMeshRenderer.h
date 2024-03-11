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
        const std::shared_ptr<MeshBuffer>& meshBuffer,
        std::shared_ptr<Shader> shader, 
        BasicCamera& camera, 
        Lighting lighting,
        std::vector<Transform3> instanceTransformations
    ) const = 0; 
    virtual void render(
        glm::vec2 screenDimensions,
        const std::vector<std::shared_ptr<MeshBuffer>>& meshBuffers,
        std::shared_ptr<Shader> shader,
        BasicCamera& camera,
        Lighting lighting,
        std::vector<Transform3> instanceTransformations
    ) const = 0;
};