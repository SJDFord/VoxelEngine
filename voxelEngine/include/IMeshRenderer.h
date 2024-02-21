#pragma once

#include <memory>
#include <vector>
#include <Shader.h>
#include <BasicCamera.h>
#include <Transform3.h>
#include <MeshBuffer.h>
#include <Lights.h>

class IMeshRenderer
{
public:
    virtual void render(
        std::shared_ptr<MeshBuffer> meshBuffer,
        std::shared_ptr<Shader> shader, 
        BasicCamera& camera, 
        Lighting lighting,
        std::vector<Transform3> instanceTransformations
    ) = 0;    // "= 0" part makes this method pure virtual, and
    // also makes this class abstract.
};