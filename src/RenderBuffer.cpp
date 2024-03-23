#include <engine/RenderBuffer.h>
#include <engine/glCheck.h>

RenderBuffer::RenderBuffer(glm::uvec2 dimensions) {
    glCheck(glGenRenderbuffers(1, &_rbo));
    glCheck(glBindRenderbuffer(GL_RENDERBUFFER, _rbo));
    glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, dimensions.x, dimensions.y)); // use a single renderbuffer object for both a depth AND stencil buffer.
    //glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo)); // now actually attach it
}

void RenderBuffer::bind() {
    glCheck(glBindRenderbuffer(GL_RENDERBUFFER, _rbo));
}

unsigned int RenderBuffer::getId() {
    return _rbo;
}

RenderBuffer::~RenderBuffer() {
    glCheck(glDeleteRenderbuffers(1, &_rbo));
}


