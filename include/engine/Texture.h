#pragma once
#include <glad/glad.h>
#include <string>

enum TextureType {
	DIFFUSE,
	SPECULAR,
	EMISSIVE
};

std::string toString(TextureType type);

class Texture
{
private:

	GLuint _id;
	TextureType _type;
	std::string _name;

	Texture(const Texture&) = default;                // 1/5: Copy Ctor
	Texture(Texture&&) noexcept = default;            // 4/5: Move Ctor
	Texture& operator=(const Texture&) = default;     // 2/5: Copy Assignment
	Texture& operator=(Texture&&) noexcept = default; // 5/5: Move Assignment
public:
	Texture(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels, TextureType type, std::string name);
	~Texture();

	void bind() const;
	GLuint getId() const;
	TextureType getType() const;
	std::string getName() const;
};