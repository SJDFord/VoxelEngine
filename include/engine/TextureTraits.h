#pragma once
#include <glad/glad.h>

struct TextureTraits
{
  typedef GLuint value_type;
  
  static value_type Create() {
    value_type handle = 0;
	glGenTextures(1, &handle);
    return handle;
  }

  static void Destroy(value_type handle) {
    glDeleteTextures(1, &handle);
  };
};