#pragma once

#include <IMeshRenderer.h>

class MeshRenderer : public IMeshRenderer {
private:
    void applyLighting(std::shared_ptr<Shader> shader, Lighting lighting) const;
public:
    void render(
        std::shared_ptr<MeshBuffer> meshBuffer,
        std::shared_ptr<Shader> shader,
        BasicCamera& camera,
        Lighting lighting,
        std::vector<Transform3> instanceTransformations
    );
};