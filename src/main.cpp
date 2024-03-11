#include <glm/glm.hpp>
#include <stdio.h>
#include <random>
#include <memory>
#include <filesystem>
#include <engine/Texture.h>
#include <engine/BasicCamera.h>
#include <engine/Vertex.h>
#include <engine/MeshBuffer.h>
#include <engine/Transform3Builder.h>
#include <engine/Material.h>
#include <engine/ShaderLoader.h>
#include <engine/TextureLoader.h>
#include <engine/glCheck.h>
#include <engine/DirectionalLightBuilder.h>
#include <engine/PointLightBuilder.h>
#include <engine/SpotLightBuilder.h>
#include <engine/IMeshRenderer.h>
#include <engine/MeshRenderer.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <engine/Window.h>
#include <engine/WindowLibrary.h>
#include <engine/GraphicsUtil.h>
#include <engine/Model.h>
#include <engine/ModelLoader.h>

const std::filesystem::path RESOURCE_FOLDER("C:/Users/sjdf/Code/VoxelEngine/resources");
std::string getResourcePath(const std::string relativePath);
void setupLighting(std::shared_ptr<Shader> shader, Lighting lighting);

// settings
glm::vec2 screenDimensions = {1600, 1200};

// camera
BasicCamera camera(Basic_Camera_Type::FPS, glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenDimensions.x / 2.0f;
float lastY = screenDimensions.y / 2.0f;
bool firstMouse = true;

// timing
const double fpsLimit = 1.0 / 60.0;
double lastUpdateTime = 0;  // number of seconds since the last loop
double lastFrameTime = 0;   // number of seconds since the last frame

//float deltaTime = 0.0f;	// time between current frame and last frame
//float lastFrame = 0.0f;

// positioning
glm::vec3 blockPos(0, 0, 0);
glm::vec3 lightPos(1.2f, 1.0f, 1.0f);

glm::vec3 lightColor;

int main(int argc, char** argv) {
    auto errorCallback = [](int code, const char* description)
        {
            fprintf(stderr, "Error: %s (%d)\n", description, code);
        };
    glfwSetErrorCallback(errorCallback);

    std::string title = "Minecraft Clone";
    std::unique_ptr<Window> window = std::make_unique<Window>(title, screenDimensions);
    window->onResized = [](glm::vec2 newSize) {
        glCheck(glViewport(0, 0, newSize.x, newSize.y));
        screenDimensions = newSize;
        };
    window->onMouseMove = [](glm::dvec2 newPosition) {

        float xpos = static_cast<float>(newPosition.x);
        float ypos = static_cast<float>(newPosition.y);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
        };

    //Model model("C:/Users/sjdf/Code/VoxelEngine/resources/models/black_dragon_with_idle_animation/scene.gltf");
    ModelLoader modelLoader;
    Model model = modelLoader.loadFromFile("C:/Users/sjdf/Code/VoxelEngine/resources/models/backpack/backpack.obj");

    ShaderLoader shaderLoader;
    std::shared_ptr<Shader> blockShader = shaderLoader.loadFromFile(getResourcePath("shaders/block.vertexshader"), getResourcePath("shaders/block.fragmentshader"));
    std::shared_ptr<Shader> lightShader = shaderLoader.loadFromFile(getResourcePath("shaders/light.vertexshader"), getResourcePath("shaders/light.fragmentshader"));
    TextureLoader textureLoader;
    std::shared_ptr<Texture> diffuseMap = textureLoader.loadFromFile(getResourcePath("textures/container2.png"), TextureType::DIFFUSE);
    std::shared_ptr<Texture> specularMap = textureLoader.loadFromFile(getResourcePath("textures/container2_specular.png"), TextureType::SPECULAR);
    std::shared_ptr<Texture> emissionMap = textureLoader.loadFromFile(getResourcePath("textures/matrix.jpg"), TextureType::EMISSIVE);
    // TODO: Create builder for Material
    Material material = { diffuseMap, specularMap, nullptr, 64.0f };
    DirectionalLightBuilder dirLightBuilder = DirectionalLightBuilder();
    dirLightBuilder
        .setProperties(glm::vec3(1.0), glm::vec3(1.0), glm::vec3(1.0))
        .setDirection(glm::vec3(1.0))
        .build();

    Mesh blockMesh = Meshes::Cube;
    Mesh lightMesh = Meshes::Cube;

    blockMesh.Textures = { diffuseMap, specularMap };
    std::shared_ptr<MeshBuffer> blockMeshBuffer = createMeshBuffer(blockMesh);
    std::shared_ptr<MeshBuffer> lightMeshBuffer = createMeshBuffer(lightMesh);
    std::vector<std::shared_ptr<MeshBuffer>> modelMeshBuffers = createMeshBuffers(model);
    std::vector<glm::vec3> cubePositions = {
        //glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    std::vector<Transform3> blockTransformations = {};
    for (int i = 0; i < cubePositions.size(); i++) {
        float angle = 20.0f * i;
        Transform3 blockTransformation = Transform3Builder()
            //.rotateAroundYAxis(glm::radians(45.0f))
            .rotateAroundAxis(glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f))
            .translateTo(cubePositions[i])
            .build();

        blockTransformations.push_back(blockTransformation);
    }

    Transform3 modelTransform = Transform3Builder()
        .translateTo(glm::vec3(0.0f, 0.0f, 0.0f))
        .build();

    std::vector<glm::vec3> pointLightPositions = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    std::vector<Transform3> pointLightTransformations;

    for (int i = 0; i < pointLightPositions.size(); i++) {
        Transform3 transform = Transform3Builder()
            .scaleBy(0.2)
            .translateTo(pointLightPositions[i])
            .build();

        pointLightTransformations.push_back(transform);

    }
    
   
    IMeshRenderer *meshRenderer = new MeshRenderer();

	do {
        double now = glfwGetTime();
        double deltaTime = now - lastUpdateTime;

        WindowLibrary::pollEvents();

        // input
        // -----
        if (window->isKeyPressed(WindowKey::WINDOW_KEY_ESCAPE)) {
            window->setShouldClose(true);
        }
        if (window->isKeyPressed(WindowKey::WINDOW_KEY_W)) {
            camera.ProcessKeyboard(Basic_Camera_Movement::FORWARD, deltaTime);    
        }
        if (window->isKeyPressed(WindowKey::WINDOW_KEY_S)) {
            camera.ProcessKeyboard(Basic_Camera_Movement::BACKWARD, deltaTime);   
        }
        if (window->isKeyPressed(WindowKey::WINDOW_KEY_A)) {
            camera.ProcessKeyboard(Basic_Camera_Movement::LEFT, deltaTime);    
        }
        if (window->isKeyPressed(WindowKey::WINDOW_KEY_D)) {
            camera.ProcessKeyboard(Basic_Camera_Movement::RIGHT, deltaTime);    
        }
        if (window->isKeyPressed(WindowKey::WINDOW_KEY_SPACE)) {
            camera.ProcessKeyboard(Basic_Camera_Movement::UP, deltaTime);    
        }
        if (window->isKeyPressed(WindowKey::WINDOW_KEY_LEFT_SHIFT)) {
            camera.ProcessKeyboard(Basic_Camera_Movement::DOWN, deltaTime);    
        }
        double frameDelta = now - lastFrameTime;
        if (frameDelta >= fpsLimit) {
            float fps = 1.0f / frameDelta;
            int fpsInt = std::round(fps);
            fprintf(stdout, "FPS: %i\n", fpsInt);

            // render
            // ------
            glCheck(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
            glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            Lighting lighting = { {}, {}, {} };
            DirectionalLightBuilder builder = DirectionalLightBuilder();
            DirectionalLight dirLight = builder
                .setDirection({ cos(glfwGetTime()),sin(glfwGetTime()), 0.0f })
                .setProperties({ 0.05f, 0.05f, 0.05f }, { 0.4f, 0.4f, 0.4f }, { 0.5f, 0.5f, 0.5f })
                .build();
            lighting.DirectionalLights.push_back(dirLight);

            PointLightBuilder pointLightBuilder =
                PointLightBuilder()
                .setProperties({ 0.05f, 0.05f, 0.05f }, { 0.8f, 0.8f, 0.8f }, { 1.0f, 1.0f, 1.0f })
                .setAttenuation(1.0f, 0.09f, 0.032f);

            for (int i = 0; i < pointLightPositions.size(); i++) {
                lighting.PointLights.push_back(pointLightBuilder.setPosition(pointLightPositions[i]).build());
            }

            SpotLightBuilder spotLightBuilder = SpotLightBuilder();
            SpotLight spotLight = spotLightBuilder
                .setPosition(camera.Position)
                .setDirection(camera.Front)
                .setProperties({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f })
                .setAttenuation(1.0f, 0.09f, 0.032f)
                .setCutOffs(glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)))
                .build();

            lighting.SpotLights.push_back(spotLight);


            meshRenderer->render(screenDimensions, blockMeshBuffer, blockShader, camera, lighting, blockTransformations);

            // TODO: Custom renderer for point lights
            meshRenderer->render(screenDimensions, lightMeshBuffer, lightShader, camera, lighting, pointLightTransformations);
          
            meshRenderer->render(screenDimensions, modelMeshBuffers, blockShader, camera, lighting, { modelTransform });

            //model.Draw(screenDimensions, blockShader, camera, lighting, blockTransformations);

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------    
            window->swapBuffers();

            lastFrameTime = now;
        }

        lastUpdateTime = now;
        
        //WindowLibrary::pollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (!window->shouldClose());

    delete meshRenderer;
}

std::string getResourcePath(const std::string relativePathStr) {
    std::filesystem::path relativePath(relativePathStr);
    std::filesystem::path resourcePath = RESOURCE_FOLDER / relativePath;
    return resourcePath.string();
}

void setupLighting(std::shared_ptr<Shader> shader, Lighting lighting)
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
        shader->setVec3(prefix + "position", light.Direction);
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