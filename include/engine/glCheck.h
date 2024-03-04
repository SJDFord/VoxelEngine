#pragma once
#include <glad/glad.h>
#include <iostream>
#include <filesystem>

// Derived from https://github.com/SFML/SFML/blob/master/src/SFML/Graphics/GLCheck.cpp

// In debug mode, perform a test on every OpenGL call
// The do-while loop is needed so that glCheck can be used as a single statement in if/else branches
#ifdef _DEBUG

#define glCheck(expr)                                      \
    do                                                     \
    {                                                      \
        expr;                                              \
        glCheckError(__FILE__, __LINE__, #expr); \
    } while (false)

#else

// Else, we don't add any overhead
#define glCheck(expr) (expr)

#endif


void glCheckError(const std::filesystem::path & file, unsigned int line, std::string_view expression);