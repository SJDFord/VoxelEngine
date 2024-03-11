#pragma once

#include <engine/IMeshRenderer.h>

class MeshRenderer : public IMeshRenderer {
private:
    void applyLighting(std::shared_ptr<Shader> shader, Lighting lighting) const;
public:
    void render(
        glm::vec2 screenDimensions,
        const std::shared_ptr<MeshBuffer>& meshBuffer,
        std::shared_ptr<Shader> shader,
        BasicCamera& camera,
        Lighting lighting,
        std::vector<Transform3> instanceTransformations
    ) const;
    void render(
        glm::vec2 screenDimensions,
        const std::vector<std::shared_ptr<MeshBuffer>>& meshBuffers,
        std::shared_ptr<Shader> shader,
        BasicCamera& camera,
        Lighting lighting,
        std::vector<Transform3> instanceTransformations
    ) const;
};