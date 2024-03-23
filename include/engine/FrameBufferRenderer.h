#pragma once

#include <memory>
#include <MeshBuffer.h>
#include <Shader.h>

class FrameBufferRenderer {
public:
    void render(
        const std::shared_ptr<MeshBuffer>& meshBuffer,
        std::shared_ptr<Shader> shader) const;
};