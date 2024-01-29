#pragma once
#include <glad/glad.h>
#include <string>

class Texture
{
private:

	GLuint _id;

	Texture(const Texture&) = default;                // 1/5: Copy Ctor
	Texture(Texture&&) noexcept = default;            // 4/5: Move Ctor
	Texture& operator=(const Texture&) = default;     // 2/5: Copy Assignment
	Texture& operator=(Texture&&) noexcept = default; // 5/5: Move Assignment
public:
	Texture(const std::string filename);
	~Texture();

	void bind() const;
	GLuint getId() const;
};