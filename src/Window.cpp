#include <engine/Window.h>

Window::Window(std::string title, glm::vec2 initialDimensions) {
    // glfwInit checks if already initialised so it's not an issue to call it per-window
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
        throw "Failed to initialize GLFW";
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

    _wptr = glfwCreateWindow(initialDimensions.x, initialDimensions.y, title.c_str(), nullptr, nullptr);
    if (_wptr == nullptr) {
		fprintf(stderr, "Failed to open GLFW window. If you have an older Intel GPU, they are not 3.3 compatible. Nor is a VM running in Parallels for Mac.\n");
		glfwTerminate();
        throw "Failed to open GLFW window. If you have an older Intel GPU, they are not 3.3 compatible. Nor is a VM running in Parallels for Mac.";
	}
    glfwSetWindowUserPointer(_wptr, this);

	glfwMakeContextCurrent(_wptr);

    // Make rendering dimensions update when window is resized
    auto frameBufferSizeCallback = [](GLFWwindow* window, int w, int h)
    {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->onResized({w, h});
    };

    glfwSetFramebufferSizeCallback(_wptr, frameBufferSizeCallback);

    // Capture different input types
    auto cursorPosCallback = [](GLFWwindow* window, double x, double y)
    {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->onMouseMove({x, y});
    };
    glfwSetCursorPosCallback(_wptr, cursorPosCallback);
    
    auto scrollCallback = [](GLFWwindow* window, double x, double y)
    {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->onMouseScroll({x, y});
    };
    glfwSetScrollCallback(_wptr, scrollCallback);
    glfwSetInputMode(_wptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // tell GLFW to capture our mouse

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	glfwSetInputMode(_wptr, GLFW_STICKY_KEYS, GL_TRUE);
	// Enable depth test
	//glCheck(glEnable(GL_DEPTH_TEST));
    glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	//glCheck();//
    glDepthFunc(GL_LESS);
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(_wptr) != 0;
}


void Window::setShouldClose(bool shouldClose) {
    glfwSetWindowShouldClose(_wptr, shouldClose);
}

void Window::swapBuffers() {
    glfwSwapBuffers(_wptr);
}

bool Window::isKeyPressed(WindowKey key) {
    int state = glfwGetKey(_wptr, key);
    switch (state) {
        case GLFW_PRESS:
            return true;
        case GLFW_RELEASE:
            return false;
        default:
            throw "Invalid key state";
    }
}

Window::~Window() {
    fprintf(stdout, "Destroying window %s", "test");
    glfwDestroyWindow(_wptr);
}