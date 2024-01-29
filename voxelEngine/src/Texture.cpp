#include "Texture.h"
#include <stb_image.h>

// Create a texture in the Graphics card
Texture::Texture(const std::string filename)
{
	// load image, create texture and generate mipmaps
	int width, height, channelsInFile;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channelsInFile, 0);
	
	if (!data) {
		throw "Failed to load texture";
	}
	fprintf(stdout, "Loaded texture %s width: %i, height: %i, channels: %i\n", filename.c_str(), width, height, channelsInFile);
	GLenum format;
	switch (channelsInFile) {
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

	glGenTextures(1, &this->_id);
	glBindTexture(GL_TEXTURE_2D, this->_id);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	GLint filter = GL_NEAREST; // GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	stbi_image_free(data);
}

void Texture::bind() const {

	glBindTexture(GL_TEXTURE_2D, this->_id);

}

GLuint Texture::getId() const
{
	return this->_id;
}

// Delete the texture from the graphics card
Texture::~Texture()
{
	glDeleteTextures(1, &this->_id);
	fprintf(stdout, "Texture disposed: %s\n", "placeholder");
}