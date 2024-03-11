#include "engine/Texture.h"
#include <engine/glCheck.h>

// Create a texture in the Graphics card
Texture::Texture(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels, std::string type, std::string name)
{
	_type = type;
	_name = name;
	GLenum format;
	switch (channels) {
	case 1:
		format = GL_RED;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	default:
		throw "Invalid format specified";
	}

	glCheck(glGenTextures(1, &this->_id));
	glCheck(glBindTexture(GL_TEXTURE_2D, this->_id));
	glCheck(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
	glCheck(glGenerateMipmap(GL_TEXTURE_2D));

	// set the texture wrapping parameters
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	// set texture filtering parameters
	GLint filter = GL_NEAREST; // GL_LINEAR
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter));
	fprintf(stdout, "Texture created: %s (%s)\n", name.c_str(), type.c_str());
}

void Texture::bind() const {
	glCheck(glBindTexture(GL_TEXTURE_2D, this->_id));
}

GLuint Texture::getId() const
{
	return this->_id;
}


std::string Texture::getType() const {
	return this->_type;
}

std::string Texture::getName() const {
	return this->_name;
}

// Delete the texture from the graphics card
Texture::~Texture()
{
	glCheck(glDeleteTextures(1, &this->_id));
	fprintf(stdout, "Texture disposed: %s (%s)\n", _name.c_str(), _type.c_str());
}