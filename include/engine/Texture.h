#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include "OpenGLObject.h"
#include "TextureTraits.h"

enum TextureType {
	DIFFUSE,
	SPECULAR,
	EMISSIVE
};

std::string toString(TextureType type);

enum TextureFormat {
	GRAYSCALE,
	COLOUR,
	COLOUR_WITH_TRANSPARENCY
};

enum TextureWrapMode {
	REPEAT,
	MIRRORED_REPEAT,
	CLAMP_TO_EDGE,
	CLAMP_TO_BORDER
};

struct TextureWrapping {
	TextureWrapMode HorizontalWrapMode;// GL_TEXTURE_WRAP_S
	TextureWrapMode VerticalWrapMode;// GL_TEXTURE_WRAP_T
};

enum TextureFilter {
	NEAREST,
	LINEAR
};

struct TextureFiltering {
	TextureFilter Min;
	TextureFilter Mag;
};

struct TextureProperties {
	std::string Name;
	TextureType Type;
	TextureWrapping Wrapping;
	TextureFiltering Filtering;
	bool UseMipMaps;
};

class Texture
{
private:

	GLuint _id = 0;
	glm::uvec2 _dimensions;
	TextureFormat _format;
	TextureProperties _properties;

	void Release()
	{
		glDeleteTextures(1, &_id);
		_id = 0;
	}

	Texture(const Texture&) = default;                // 1/5: Copy Ctor
	Texture& operator=(const Texture&) = default;     // 2/5: Copy Assignment

	/*
	Texture(Texture&&) noexcept = default;            // 4/5: Move Ctor
	Texture& operator=(Texture&&) noexcept = default; // 5/5: Move Assignment
	*/

	GLenum convertWrapModeToGl(TextureWrapMode mode);
	GLenum convertFilterToGl(TextureFilter filter);
public:
	Texture(unsigned char* data, glm::uvec2 dimensions, TextureFormat format, TextureProperties properties);

	Texture(Texture &&other) : _id(other._id)
	{
		other._id = 0; //Use the "null" texture for the old object.
	}

	Texture &operator=(Texture &&other)
	{
		//ALWAYS check for self-assignment.
		if(this != &other)
		{
			Release();
			//obj_ is now 0.
			std::swap(_id, other._id);
		}
	}
	
	~Texture();

	void bind() const;
	GLuint getId() const;
	glm::uvec2 getDimensions() const;
	TextureFormat getFormat() const;
	TextureProperties getProperties() const;


};