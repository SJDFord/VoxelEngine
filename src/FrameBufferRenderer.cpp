#include <engine/FrameBufferRenderer.h>

void FrameBufferRenderer::render(
    const std::shared_ptr<MeshBuffer>& meshBuffer,
    std::shared_ptr<Shader> shader
) const {

    shader->use();
    std::vector<std::shared_ptr<Texture>> textures = meshBuffer->getTextures();
    std::shared_ptr<Texture> texture = textures[0];
    glActiveTexture(GL_TEXTURE0);
    shader->setInt("screenTexture", 0);
    texture->bind();
    glActiveTexture(GL_TEXTURE0);
    meshBuffer->draw();
}