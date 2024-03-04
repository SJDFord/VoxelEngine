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

const std::filesystem::path RESOURCE_FOLDER("C:/Users/sjdf/Code/VoxelEngine/resources");

struct DestroyglfwWin {

    void operator()(GLFWwindow* ptr) {
        fprintf(stdout, "Destroying window %s", "test");
        glfwDestroyWindow(ptr);
    }

};

std::unique_ptr<GLFWwindow, DestroyglfwWin> setupEnvironment(int width, int height, const char* title);
static void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
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
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


// positioning
glm::vec3 blockPos(0, 0, 0);
glm::vec3 lightPos(1.2f, 1.0f, 1.0f);

glm::vec3 lightColor;

struct TextureCoords {
    static constexpr glm::vec2 BottomLeft = glm::vec2(0.0f, 0.0f);
    static constexpr glm::vec2 TopLeft = glm::vec2(0.0f, 1.0f);
    static constexpr glm::vec2 BottomRight = glm::vec2(1.0f, 0.0f);
    static constexpr glm::vec2 TopRight = glm::vec2(1.0f, 1.0f);
};

struct CubeCoords {
    static constexpr glm::vec3 FrontBottomLeft = glm::vec3(-0.5f, -0.5f, 0.5f);
    static constexpr glm::vec3 FrontTopLeft = glm::vec3(-0.5f, 0.5f, 0.5f);
    static constexpr glm::vec3 FrontBottomRight = glm::vec3(0.5f, -0.5f, 0.5f);
    static constexpr glm::vec3 FrontTopRight = glm::vec3(0.5f, 0.5f, 0.5f);
    
    static constexpr glm::vec3 BackBottomLeft = glm::vec3(-0.5f, -0.5f, -0.5f);
    static constexpr glm::vec3 BackTopLeft = glm::vec3(-0.5f, 0.5f, -0.5f);
    static constexpr glm::vec3 BackBottomRight = glm::vec3(0.5f, -0.5f, -0.5f);
    static constexpr glm::vec3 BackTopRight = glm::vec3(0.5f, 0.5f, -0.5f);
};

struct CubeNormals {

    static constexpr glm::vec3 Back = glm::vec3(0.0f, 0.0f, -1.0f);
    static constexpr glm::vec3 Front = glm::vec3(0.0f, 0.0f, 1.0f);
    static constexpr glm::vec3 Left = glm::vec3(-1.0f, 0.0f, 0.0f);
    static constexpr glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
    static constexpr glm::vec3 Bottom = glm::vec3(0.0f, -1.0f, 0.0f);
    static constexpr glm::vec3 Top = glm::vec3(0.0f, 1.0f, 0.0f);
};

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

    ShaderLoader shaderLoader;
    std::shared_ptr<Shader> blockShader = shaderLoader.loadFromFile(getResourcePath("shaders/block.vertexshader"), getResourcePath("shaders/block.fragmentshader"));
    std::shared_ptr<Shader> lightShader = shaderLoader.loadFromFile(getResourcePath("shaders/light.vertexshader"), getResourcePath("shaders/light.fragmentshader"));
    TextureLoader textureLoader;
    std::shared_ptr<Texture> diffuseMap = textureLoader.loadFromFile(getResourcePath("textures/container2.png"));
    std::shared_ptr<Texture> specularMap = textureLoader.loadFromFile(getResourcePath("textures/container2_specular.png"));
    std::shared_ptr<Texture> emissionMap = textureLoader.loadFromFile(getResourcePath("textures/matrix.jpg"));
    // TODO: Create builder for Material
    Material material = { diffuseMap, specularMap, emissionMap, 64.0f };
    DirectionalLightBuilder dirLightBuilder = DirectionalLightBuilder();
    dirLightBuilder
        .setProperties(glm::vec3(1.0), glm::vec3(1.0), glm::vec3(1.0))
        .setDirection(glm::vec3(1.0))
        .build();



    Vertex v = { CubeCoords::BackBottomLeft, CubeNormals::Back, TextureCoords::BottomRight };
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<Vertex> cubeVertices = {
        // Back
        {CubeCoords::BackBottomLeft, CubeNormals::Back, TextureCoords::BottomRight},
        {CubeCoords::BackBottomRight, CubeNormals::Back, TextureCoords::BottomLeft},
        {CubeCoords::BackTopRight, CubeNormals::Back, TextureCoords::TopLeft},

        {CubeCoords::BackTopRight, CubeNormals::Back, TextureCoords::TopLeft},
        {CubeCoords::BackTopLeft, CubeNormals::Back, TextureCoords::TopRight},
        {CubeCoords::BackBottomLeft, CubeNormals::Back, TextureCoords::BottomRight},

        // Front
        {CubeCoords::FrontBottomLeft, CubeNormals::Front, TextureCoords::BottomLeft},
        {CubeCoords::FrontBottomRight, CubeNormals::Front, TextureCoords::BottomRight},
        {CubeCoords::FrontTopRight, CubeNormals::Front, TextureCoords::TopRight},

        {CubeCoords::FrontTopRight, CubeNormals::Front, TextureCoords::TopRight},
        {CubeCoords::FrontTopLeft, CubeNormals::Front, TextureCoords::TopLeft},
        {CubeCoords::FrontBottomLeft, CubeNormals::Front,TextureCoords::BottomLeft},

        // Left
        {CubeCoords::FrontTopLeft, CubeNormals::Left, TextureCoords::TopRight},
        {CubeCoords::BackTopLeft,  CubeNormals::Left, TextureCoords::TopLeft},
        {CubeCoords::BackBottomLeft,  CubeNormals::Left, TextureCoords::BottomLeft},

        {CubeCoords::BackBottomLeft, CubeNormals::Left, TextureCoords::BottomLeft},
        {CubeCoords::FrontBottomLeft, CubeNormals::Left, TextureCoords::BottomRight},
        {CubeCoords::FrontTopLeft, CubeNormals::Left, TextureCoords::TopRight},

        // Right
        {CubeCoords::FrontTopRight, CubeNormals::Right, TextureCoords::TopLeft},
        {CubeCoords::BackTopRight, CubeNormals::Right, TextureCoords::TopRight},
        {CubeCoords::BackBottomRight, CubeNormals::Right, TextureCoords::BottomRight},

        {CubeCoords::BackBottomRight, CubeNormals::Right, TextureCoords::BottomRight},
        {CubeCoords::FrontBottomRight, CubeNormals::Right, TextureCoords::BottomLeft},
        {CubeCoords::FrontTopRight, CubeNormals::Right, TextureCoords::TopLeft},

        // Bottom
        {CubeCoords::BackBottomLeft, CubeNormals::Bottom, TextureCoords::BottomLeft},
        {CubeCoords::BackBottomRight, CubeNormals::Bottom, TextureCoords::BottomRight},
        {CubeCoords::FrontBottomRight, CubeNormals::Bottom, TextureCoords::TopRight},

        {CubeCoords::FrontBottomRight, CubeNormals::Bottom, TextureCoords::TopRight},
        {CubeCoords::FrontBottomLeft, CubeNormals::Bottom, TextureCoords::TopLeft},
        {CubeCoords::BackBottomLeft, CubeNormals::Bottom,TextureCoords::BottomLeft},

        // Top
        {CubeCoords::BackTopLeft, CubeNormals::Top, TextureCoords::TopLeft},
        {CubeCoords::BackTopRight, CubeNormals::Top, TextureCoords::TopRight},
        {CubeCoords::FrontTopRight, CubeNormals::Top, TextureCoords::BottomRight},

        {CubeCoords::FrontTopRight, CubeNormals::Top, TextureCoords::BottomRight},
        {CubeCoords::FrontTopLeft, CubeNormals::Top, TextureCoords::BottomLeft},
        {CubeCoords::BackTopLeft, CubeNormals::Top, TextureCoords::TopLeft}
    };


    Mesh blockMesh = { cubeVertices };
    Mesh lightMesh = { cubeVertices };
    std::shared_ptr<MeshBuffer> blockMeshBuffer = std::make_shared<MeshBuffer>(blockMesh, &material);
    std::shared_ptr<MeshBuffer> lightMeshBuffer = std::make_shared<MeshBuffer>(lightMesh);

    std::vector<glm::vec3> cubePositions = {
        glm::vec3(0.0f,  0.0f,  0.0f),
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
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        //processInput(window.get());
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
        // render
        // ------
        glCheck(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        
        //lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        //lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

        // 0, -1, 0
        // 1, -1, 0
        // 0, 1, 0
        // -1, 1, 0

        Lighting lighting = { {}, {}, {} };
        DirectionalLightBuilder builder = DirectionalLightBuilder();
        DirectionalLight dirLight = builder
            .setDirection({ cos(glfwGetTime()),sin(glfwGetTime()), 0.0f })
            .setProperties({ 0.05f, 0.05f, 0.05f }, { 0.4f, 0.4f, 0.4f }, { 0.5f, 0.5f, 0.5f })
            .build();
        lighting.DirectionalLights.push_back(dirLight);

        PointLightBuilder pointLightBuilder =
            PointLightBuilder()
            .setProperties({ 0.05f, 0.05f, 0.05f }, { 0.8f, 0.8f, 0.8f }, { .0f, 1.0f, 1.0f })
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

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        
        window->swapBuffers();
        WindowLibrary::pollEvents();
        /*
        glfwSwapBuffers(window.get());
        glfwPollEvents();
        */
       /*glfwGetKey(window.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS*/

	} // Check if the ESC key was pressed or the window was closed
	while (!window->shouldClose());

    delete meshRenderer;
}

std::unique_ptr<GLFWwindow, DestroyglfwWin> setupEnvironment(int width, int height, const char* title)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
        throw "Failed to initialize GLFW";
	}

	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

    GLFWwindow* wptr = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (wptr == nullptr) {
		fprintf(stderr, "Failed to open GLFW window. If you have an older Intel GPU, they are not 3.3 compatible. Nor is a VM running in Parallels for Mac.\n");
		glfwTerminate();
        throw "Failed to open GLFW window. If you have an older Intel GPU, they are not 3.3 compatible. Nor is a VM running in Parallels for Mac.";
	}

    std::unique_ptr<GLFWwindow, DestroyglfwWin> window(wptr);
	glfwMakeContextCurrent(window.get());
    // Make rendering dimensions update when window is resized
    glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);
    // Capture different input types
    glfwSetCursorPosCallback(window.get(), mouse_callback);
    glfwSetScrollCallback(window.get(), scroll_callback);
    glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); // tell GLFW to capture our mouse

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	glfwSetInputMode(window.get(), GLFW_STICKY_KEYS, GL_TRUE);
	// Enable depth test
	glCheck(glEnable(GL_DEPTH_TEST));
	// Accept fragment if it closer to the camera than the former one
	glCheck(glDepthFunc(GL_LESS));
	return window;
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    fprintf(stdout, "Window resized to %ix%i\n", width, height);
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glCheck(glViewport(0, 0, width, height));
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

/*
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
*/
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