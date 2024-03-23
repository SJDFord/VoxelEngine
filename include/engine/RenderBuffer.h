#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class RenderBuffer {
private:
    unsigned int _rbo;
    
	RenderBuffer(const RenderBuffer&) = default;                // 1/5: Copy Ctor
	RenderBuffer(RenderBuffer&&) noexcept = default;            // 4/5: Move Ctor
	RenderBuffer& operator=(const RenderBuffer&) = default;     // 2/5: Copy Assignment
	RenderBuffer& operator=(RenderBuffer&&) noexcept = default; // 5/5: Move Assignment
public:
    RenderBuffer(glm::uvec2 dimensions);
    void bind();
    unsigned int getId();
    ~RenderBuffer();

};