#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "VoxelEngine.h"
#include "MouseAndKeyboardCameraControllerInput.h"
#include "SimpleLight.h"
#include <glm/glm.hpp>
#include <stdio.h>
#include "ISprite.h"
#include <random>
#include <filesystem>
#include "PerlinNoiseChunkFactory.h"
#define STB_IMAGE_IMPLEMENTATION    
#include <stb_image.h>
#include <Texture.h>
#include <BasicCamera.h>
#include <Vertex.h>
#include <Mesh.h>
#include <MeshTransformationsBuilder.h>

const std::filesystem::path RESOURCE_FOLDER("C:/Users/sjdf/Code/VoxelEngine/examples/minecraftClone/resources");

bool setupEnvironment(GLFWwindow*& window, int width, int height, const char* title);
static void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
std::string getResourcePath(const std::string relativePath);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

// camera
BasicCamera camera(Basic_Camera_Type::FPS, glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


// positioning
glm::vec3 blockPos(0, 0, 0);
glm::vec3 lightPos = glm::vec3(0.25f, 0.75f, 0.25f);

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

int main(int argc, char** argv) {
    // TODO: Re-implement textured cubes with camera movement example/tutorial from learnopengl
    // Then start to introduce cubes arranged in a grid and chunks etc rather than trying to apply textures to the existing voxel engine cubes

    GLFWwindow* window;
    if (setupEnvironment(window, SCR_WIDTH, SCR_HEIGHT, "Minecraft Clone")) return -1;

    Shader blockShader(getResourcePath("shaders/block.vertexshader"), getResourcePath("shaders/block.fragmentshader"));
    Shader lightShader(getResourcePath("shaders/light.vertexshader"), getResourcePath("shaders/light.fragmentshader"));
    Texture texture(getResourcePath("textures/stone.png"));


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<Vertex> cubeVertices = {
        // Back
        {CubeCoords::BackBottomLeft, TextureCoords::BottomRight},
        {CubeCoords::BackBottomRight, TextureCoords::BottomLeft},
        {CubeCoords::BackTopRight, TextureCoords::TopLeft},

        {CubeCoords::BackTopRight, TextureCoords::TopLeft},
        {CubeCoords::BackTopLeft, TextureCoords::TopRight},
        {CubeCoords::BackBottomLeft,TextureCoords::BottomRight},

        // Front
        {CubeCoords::FrontBottomLeft, TextureCoords::BottomLeft},
        {CubeCoords::FrontBottomRight, TextureCoords::BottomRight},
        {CubeCoords::FrontTopRight, TextureCoords::TopRight},

        {CubeCoords::FrontTopRight, TextureCoords::TopRight},
        {CubeCoords::FrontTopLeft, TextureCoords::TopLeft},
        {CubeCoords::FrontBottomLeft,TextureCoords::BottomLeft},

        // Left
        {CubeCoords::FrontTopLeft, TextureCoords::TopRight},
        {CubeCoords::BackTopLeft, TextureCoords::TopLeft},
        {CubeCoords::BackBottomLeft, TextureCoords::BottomLeft},

        {CubeCoords::BackBottomLeft, TextureCoords::BottomLeft},
        {CubeCoords::FrontBottomLeft, TextureCoords::BottomRight},
        {CubeCoords::FrontTopLeft,TextureCoords::TopRight},

        // Right
        {CubeCoords::FrontTopRight, TextureCoords::TopLeft},
        {CubeCoords::BackTopRight, TextureCoords::TopRight},
        {CubeCoords::BackBottomRight, TextureCoords::BottomRight},

        {CubeCoords::BackBottomRight, TextureCoords::BottomRight},
        {CubeCoords::FrontBottomRight, TextureCoords::BottomLeft},
        {CubeCoords::FrontTopRight,TextureCoords::TopLeft},

        // Bottom
        {CubeCoords::BackBottomLeft, TextureCoords::BottomLeft},
        {CubeCoords::BackBottomRight, TextureCoords::BottomRight},
        {CubeCoords::FrontBottomRight, TextureCoords::TopRight},

        {CubeCoords::FrontBottomRight, TextureCoords::TopRight},
        {CubeCoords::FrontBottomLeft, TextureCoords::TopLeft},
        {CubeCoords::BackBottomLeft,TextureCoords::BottomLeft},

        // Top
        {CubeCoords::BackTopLeft, TextureCoords::TopLeft},
        {CubeCoords::BackTopRight, TextureCoords::TopRight},
        {CubeCoords::FrontTopRight, TextureCoords::BottomRight},

        {CubeCoords::FrontTopRight, TextureCoords::BottomRight},
        {CubeCoords::FrontTopLeft, TextureCoords::BottomLeft},
        {CubeCoords::BackTopLeft,TextureCoords::TopLeft}
    };


    Mesh blockMesh(cubeVertices);
    Mesh lightMesh(cubeVertices);

    MeshTransformations blockTransformations = MeshTransformationsBuilder()
        .rotateAroundYAxis(glm::radians(45.0f))
        .translateTo(blockPos)
        .build();
    MeshTransformations lightTransformations = MeshTransformationsBuilder()
        .scaleBy(0.5)
        .translateTo(lightPos)
        .build();


	do {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::function<void(const Shader&)> setupBlockShader = [](const Shader& s) { 
            s.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
            s.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        };
        blockMesh.render(blockShader, camera, { blockTransformations }, setupBlockShader);
        lightMesh.render(lightShader, camera, { lightTransformations });

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
    
	glfwTerminate();
}

bool setupEnvironment(GLFWwindow*& window, int width, int height, const char* title)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return true;
	}

	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (window == nullptr) {
		fprintf(stderr, "Failed to open GLFW window. If you have an older Intel GPU, they are not 3.3 compatible. Nor is a VM running in Parallels for Mac.\n");
		glfwTerminate();
		return true;
	}
	glfwMakeContextCurrent(window);
    // Make rendering dimensions update when window is resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Capture different input types
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // tell GLFW to capture our mouse

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	return false;
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
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
}

std::string getResourcePath(const std::string relativePathStr) {
    std::filesystem::path relativePath(relativePathStr);
    std::filesystem::path resourcePath = RESOURCE_FOLDER / relativePath;
    return resourcePath.string();
}