#include <engine/WindowLibrary.h>
#include <glfw/glfw3.h>
#include <iostream>

void WindowLibrary::pollEvents() {
    glfwPollEvents();
}