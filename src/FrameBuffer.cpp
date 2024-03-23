#include <engine/FrameBuffer.h>
#include <glad/glad.h>
#include <engine/glCheck.h>

FrameBuffer::FrameBuffer(std::shared_ptr<Texture> textureColourBuffer, std::shared_ptr<RenderBuffer> renderBuffer) {
    glCheck(glGenFramebuffers(1, &_fbo));

    bind();
    glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColourBuffer->getId(), 0));
    glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer->getId())); 

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw "Frame buffer not complete";
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FrameBuffer::getId() {
    return _fbo;
}

void FrameBuffer::bind() {
    glCheck(glBindFramebuffer(GL_FRAMEBUFFER, _fbo));  
}

FrameBuffer::~FrameBuffer() {

    glCheck(glDeleteFramebuffers(1, &_fbo));
}