#include "engine/Texture.h"
#include <engine/glCheck.h>

std::string toString(TextureType type) {
	switch (type) {
	case DIFFUSE:
		return "DIFFUSE";
	case SPECULAR:
		return "SPECULAR";
	case EMISSIVE:
		return "EMISSIVE";
	default:
		throw "Type string not implemented for type " + std::to_string(type);
	}
}

// Create a texture in the Graphics card
Texture::Texture(unsigned char* data, glm::uvec2 dimensions, TextureFormat format, TextureProperties properties)
{
	_dimensions = dimensions;
	_format = format;
	_properties = properties;

	GLenum glFormat;
	switch (format) {
	case TextureFormat::GRAYSCALE:
		glFormat = GL_RED;
		break;
	case TextureFormat::COLOUR:
		glFormat = GL_RGB;
		break;
	case TextureFormat::COLOUR_WITH_TRANSPARENCY:
		glFormat = GL_RGBA;
		break;
	default:
		throw "Invalid format specified";
	}

	glCheck(glGenTextures(1, &this->_id));
	glCheck(glBindTexture(GL_TEXTURE_2D, this->_id));
	glCheck(glTexImage2D(GL_TEXTURE_2D, 0, glFormat, dimensions.x, dimensions.y, 0, glFormat, GL_UNSIGNED_BYTE, data));
	
	// set the texture wrapping parameters

	TextureWrapping wrapping = properties.Wrapping;
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, convertWrapModeToGl(wrapping.HorizontalWrapMode)));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, convertWrapModeToGl(wrapping.VerticalWrapMode)));

	TextureFiltering filtering = properties.Filtering;
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, convertFilterToGl(filtering.Min)));
	glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, convertFilterToGl(filtering.Mag)));

	if (properties.UseMipMaps) {
		glCheck(glGenerateMipmap(GL_TEXTURE_2D));
	}

	fprintf(stdout, "Texture created: %s (%s)\n", this->_properties.Name.c_str(), toString(this->_properties.Type).c_str());
}

void Texture::bind() const {
	glCheck(glBindTexture(GL_TEXTURE_2D, this->_id));
}

GLuint Texture::getId() const
{
	return this->_id;
}


glm::uvec2 Texture::getDimensions() const
{
	return this->_dimensions;
}

TextureFormat Texture::getFormat() const
{
	return this->_format;
}

TextureProperties Texture::getProperties() const
{
	return this->_properties;
}

// Delete the texture from the graphics card
Texture::~Texture()
{
	this->Release();
	//glCheck(glDeleteTextures(1, &this->_id));
	//fprintf(stdout, "Texture disposed: %s (%s)\n", this->_properties.Name.c_str(), toString(this->_properties.Type).c_str());
}


GLenum Texture::convertWrapModeToGl(TextureWrapMode mode) {
	switch (mode) {
	case TextureWrapMode::REPEAT:
		return GL_REPEAT;
	case TextureWrapMode::MIRRORED_REPEAT:
		return GL_MIRRORED_REPEAT;

	case TextureWrapMode::CLAMP_TO_BORDER:
		return GL_CLAMP_TO_BORDER;

	case TextureWrapMode::CLAMP_TO_EDGE:
		return GL_CLAMP_TO_EDGE;
	default:
		throw "Wrap mode not supported";
	}
}


GLenum Texture::convertFilterToGl(TextureFilter filter) {
	switch (filter) {
	case NEAREST:
		return GL_NEAREST;
	case LINEAR:
		return GL_LINEAR;
	default:
		throw "Filter not supported";
	}
}