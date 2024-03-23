#pragma once
#include <memory>
#include <engine/Texture.h>
#include <engine/RenderBuffer.h>

class FrameBuffer {
private:
    unsigned int _fbo;
    
    
	FrameBuffer(const FrameBuffer&) = default;                // 1/5: Copy Ctor
	FrameBuffer(FrameBuffer&&) noexcept = default;            // 4/5: Move Ctor
	FrameBuffer& operator=(const FrameBuffer&) = default;     // 2/5: Copy Assignment
	FrameBuffer& operator=(FrameBuffer&&) noexcept = default; // 5/5: Move Assignment
public:
	FrameBuffer(std::shared_ptr<Texture> textureColourBuffer, std::shared_ptr<RenderBuffer> renderBuffer);
    void bind();
    unsigned int getId();
	~FrameBuffer();
};