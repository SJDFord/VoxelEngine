#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "VoxelEngine.h"
#include "MouseAndKeyboardCameraControllerInput.h"
#include "SimpleLight.h"
#include <glm/glm.hpp>
#include <stdio.h>
#include "ISprite.h"
#include <random>
#include "PerlinNoiseChunkFactory.h"
#define STB_IMAGE_IMPLEMENTATION    
#include <stb_image.h>
#include <Texture.h>
#include <BasicCamera.h>
#include <Vertex.h>
#include <Mesh.h>

bool setupEnvironment(GLFWwindow*& window, int width, int height, const char* title);
static void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

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

	const std::string vertexShaderFile = "C:/Users/sjdf/Code/SimpleVoxelEngine/examples/minecraftClone/resources/shaders/vertex.vertexshader";
	const std::string fragmentShaderFile = "C:/Users/sjdf/Code/SimpleVoxelEngine/examples/minecraftClone/resources/shaders/fragment.fragmentshader";
	Shader shader(vertexShaderFile, fragmentShaderFile);

    

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    TextureCoords::BottomLeft;
    std::vector<Vertex> verticesNew = {
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

    float vertices[] = {
        // positions          // texture coords
        // back
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Bottom left
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom right
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Top right

         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // Top right
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // Top left
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Bottom left

        // front
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        
        // left
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top - front
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top - back
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom - back

        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom - back
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom - front
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top - front

        
        // right
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top front
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top back
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom back

         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom back
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom front
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top front

        // bottom
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // back left
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // back right
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // front right

         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // front right
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // front left
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // back left

        // top
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float verticesGenerated[36 * 5] = {};

    for (unsigned int i = 0; i < verticesNew.size(); i++) {
        Vertex& vertex = verticesNew.at(i);
        float x = vertex.Position.x;
        float y = vertex.Position.y;
        float z = vertex.Position.z;
        float s = vertex.TexCoords.x;
        float t = vertex.TexCoords.y;
        verticesGenerated[5 * i] = x;
        verticesGenerated[5 * i + 1] = y;
        verticesGenerated[5 * i + 2] = z;
        verticesGenerated[5 * i + 3] = s;
        verticesGenerated[5 * i + 4] = t;
    }

    // world space positions of our cubes
    std::vector<glm::vec3> cubePositions;
    /*
    [] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(1.0f,  0.0f,  0.0f),

        glm::vec3(0.0f,  3.0f,  0.0f),
        glm::vec3(2.0f,  3.0f,  0.0f),
        //glm::vec3(-1.5f, -2.2f, -2.5f),
        //glm::vec3(-3.8f, -2.0f, -12.3f),
        //glm::vec3(2.4f, -0.4f, -3.5f),
        //glm::vec3(-1.7f,  3.0f, -7.5f),
        //glm::vec3(1.3f, -2.0f, -2.5f),
        //glm::vec3(1.5f,  2.0f, -2.5f),
        //glm::vec3(1.5f,  0.2f, -1.5f),
        //glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    */

    unsigned int chunkSize = 1;
    for (unsigned int x = 0; x < chunkSize; x++) {
        for (unsigned int y = 0; y < chunkSize; y++) {
            for (unsigned int z = 0; z < chunkSize; z++) {
                float xPos = x;
                float yPos = y;
                float zPos = z;
                cubePositions.push_back(glm::vec3(x, y, z));
            }
        }
    }

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesGenerated), verticesGenerated, GL_STATIC_DRAW);

    // position attribute
    // Index (shader location value), Size (number of values), Type of values, Stride, Pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    const std::string imageFile = "C:/Users/sjdf/Code/SimpleVoxelEngine/examples/minecraftClone/resources/textures/stone.png";
    //const std::string imageFile = "C:/Users/sjdf/Code/SimpleVoxelEngine/examples/minecraftClone/resources/textures/container.jpg";
    //const std::string imageFile = "C:/Users/sjdf/Code/SimpleVoxelEngine/examples/minecraftClone/resources/textures/seamless_brick-512x512.png";
    Texture texture(imageFile);

    Mesh mesh(verticesNew, &texture, cubePositions);  //(verticesNew, texture, cubePositions);

    /*
    shader.use();
    shader.setInt("textureSampler", 0);
    shader.setBool("isGreyscale", true); // Needed for single channel images 
    */

	// Configure the voxel engine to display voxels based on a perlin noise algorithm and connect the camera to the mouse and keyboard

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

        // bind textures on corresponding texture units
        /*
        glActiveTexture(GL_TEXTURE0);
        texture.bind();
        // activate shader
        shader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < chunkSize * chunkSize * chunkSize; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 0;
            //float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        */
        mesh.render(shader, camera);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
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