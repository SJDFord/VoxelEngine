#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <string>
#include <functional>
#include <iostream>
#include <glm/glm.hpp>
#include "WindowKey.h"

class Window
{
private:
    GLFWwindow* _wptr;
    
	Window(const Window&) = default;                // 1/5: Copy Ctor
	Window(Window&&) noexcept = default;            // 4/5: Move Ctor
	Window& operator=(const Window&) = default;     // 2/5: Copy Assignment
	Window& operator=(Window&&) noexcept = default; // 5/5: Move Assignment
public:
    std::function<void(glm::vec2)> onResized = [](glm::vec2) { };
    std::function<void(glm::dvec2)> onMouseMove = [](glm::dvec2) { };
    std::function<void(glm::dvec2)> onMouseScroll = [](glm::dvec2) { };


	Window(std::string title, glm::vec2 initialDimensions);

    bool shouldClose();
    void setShouldClose(bool shouldClose);
    void swapBuffers();

    bool isKeyPressed(WindowKey key);

	~Window();

};