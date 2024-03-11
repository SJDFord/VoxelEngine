#include <engine/MeshRenderer.h>
#include <engine/glCheck.h>

void MeshRenderer::render(
    glm::vec2 screenDimensions,
    const std::shared_ptr<MeshBuffer>& meshBuffer,
    std::shared_ptr<Shader> shader,
    BasicCamera& camera,
    Lighting lighting,
    std::vector<Transform3> instanceTransformations
) const {
    shader->use();
    
    bool diffuseFound = false;
    bool specularFound = false;
	std::vector<std::shared_ptr<Texture>> textures = meshBuffer->getTextures();
    shader->setFloat("material.shininess", 64.0f);
    for (int i = 0; i < textures.size(); i++) {  
        glCheck(glActiveTexture(GL_TEXTURE0 + i));
        std::shared_ptr<Texture> texture = textures[i];
        TextureType textureType = texture->getType();
        if(textureType == TextureType::DIFFUSE && !diffuseFound) {
            shader->setInt("material.diffuse", i);  
            texture->bind();  
            diffuseFound = true;
        }
        else if(textureType == TextureType::SPECULAR && !specularFound) {
            shader->setInt("material.specular", i);
            texture->bind();
            specularFound = true;
        }
    }
    glCheck(glActiveTexture(GL_TEXTURE0));

    shader->setVec3("viewPos", camera.Position);
    applyLighting(shader, lighting);


    glm::mat4 projection = glm::perspective(
        glm::radians(camera.Zoom), 
        screenDimensions.x / screenDimensions.y, 
        0.1f, 
        100.0f
    );
    shader->setMat4("projection", projection);

    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    shader->setMat4("view", view);

    // TODO: Optimise this so we can just generate one mesh for all instances (one render call)
    // render mesh instances
    for (unsigned int i = 0; i < instanceTransformations.size(); i++)
    {
        Transform3 transformation = instanceTransformations[i];

        glm::vec3 position = transformation.Position;
        float rotationAngle = transformation.RotationAngle;
        glm::vec3 rotationAxis = transformation.RotationAxis;
        glm::vec3 scaleFactor = transformation.ScaleFactor;

        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        // Scale, rotation then translation done in this order to minimize unwanted effects
        //fprintf(stdout, "Translating by %f, %f, %f, Rotating by %f, Scaling by %f, %f, %f\n", position.x, position.y, position.z, rotationAngle, scaleFactor.x, scaleFactor.y, scaleFactor.z);
        model = glm::translate(model, transformation.Position);
        model = glm::rotate(model, transformation.RotationAngle, transformation.RotationAxis);
        model = glm::scale(model, transformation.ScaleFactor);

        // glm::radians(angle)

        shader->setMat4("model", model);
        // TODO: Make this configurable as some models can't be face culled
        glCheck(glEnable(GL_CULL_FACE));
        meshBuffer->draw();
        glCheck(glDisable(GL_CULL_FACE));
    }
}


void MeshRenderer::render(
    glm::vec2 screenDimensions,
    const std::vector<std::shared_ptr<MeshBuffer>>& meshBuffers,
    std::shared_ptr<Shader> shader,
    BasicCamera& camera,
    Lighting lighting,
    std::vector<Transform3> instanceTransformations) const
{
    for (int i = 0; i < meshBuffers.size(); i++) {
        render(screenDimensions, meshBuffers[i], shader, camera, lighting, instanceTransformations);
    }
}

void MeshRenderer::applyLighting(std::shared_ptr<Shader> shader, Lighting lighting) const
{
    int dirLightCount = lighting.DirectionalLights.size();
    shader->setInt("dirLightCount", dirLightCount);
    for (int i = 0; i < dirLightCount; i++) {
        DirectionalLight light = lighting.DirectionalLights[i];
        std::string prefix = "dirLights[" + std::to_string(i) + "].";
        shader->setVec3(prefix + "direction", light.Direction);
        shader->setVec3(prefix + "ambient", light.Properties.Ambient);
        shader->setVec3(prefix + "diffuse", light.Properties.Diffuse);
        shader->setVec3(prefix + "specular", light.Properties.Specular);
    }

    int pointLightCount = lighting.PointLights.size();
    shader->setInt("pointLightCount", pointLightCount);
    for (int i = 0; i < pointLightCount; i++) {
        PointLight light = lighting.PointLights[i];
        std::string prefix = "pointLights[" + std::to_string(i) + "].";
        shader->setVec3(prefix + "position", light.Position);
        shader->setVec3(prefix + "ambient", light.Properties.Ambient);
        shader->setVec3(prefix + "diffuse", light.Properties.Diffuse);
        shader->setVec3(prefix + "specular", light.Properties.Specular);

        shader->setFloat(prefix + "constant", light.Attenuation.Constant);
        shader->setFloat(prefix + "linear", light.Attenuation.Linear);
        shader->setFloat(prefix + "quadratic", light.Attenuation.Quadratic);
    }

    int spotLightCount = lighting.SpotLights.size();
    shader->setInt("spotLightCount", spotLightCount);
    for (int i = 0; i < spotLightCount; i++) {
        SpotLight light = lighting.SpotLights[i];
        std::string prefix = "spotLights[" + std::to_string(i) + "].";
        shader->setVec3(prefix + "position", light.Position);
        shader->setVec3(prefix + "direction", light.Direction);
        shader->setVec3(prefix + "ambient", light.Properties.Ambient);
        shader->setVec3(prefix + "diffuse", light.Properties.Diffuse);
        shader->setVec3(prefix + "specular", light.Properties.Specular);

        shader->setFloat(prefix + "constant", light.Attenuation.Constant);
        shader->setFloat(prefix + "linear", light.Attenuation.Linear);
        shader->setFloat(prefix + "quadratic", light.Attenuation.Quadratic);

        shader->setFloat(prefix + "cutOff", light.CutOff);
        shader->setFloat(prefix + "outerCutOff", light.OuterCutOff);
    }
}