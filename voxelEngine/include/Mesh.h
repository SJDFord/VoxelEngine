#pragma once
#include <glad/glad.h>
#include <string>
#include <Vertex.h>
#include <Texture.h>
#include <vector>
#include <Shader.h>
#include <BasicCamera.h>

class Mesh
{
private:
	std::vector<Vertex> _vertices;
	//std::vector<unsigned int> indices; // TODO: EBOs - see https://learnopengl.com/Getting-started/Hello-Triangle#:~:text=An%20EBO%20is%20a%20buffer,the%20solution%20to%20our%20problem.
	// Note also that for faces with different colours/textures we can't use EBOs
	const Texture* _texture;
	std::vector<glm::vec3> _positions;
	unsigned int _vao;
	unsigned int _vbo;

	Mesh(const Mesh&) = default;                // 1/5: Copy Ctor
	Mesh(Mesh&&) noexcept = default;            // 4/5: Move Ctor
	Mesh& operator=(const Mesh&) = default;     // 2/5: Copy Assignment
	Mesh& operator=(Mesh&&) noexcept = default; // 5/5: Move Assignment
public:
	Mesh(
		const std::vector<Vertex> vertices, 
		const Texture* texture, 
		const std::vector<glm::vec3> positions // World space coordinates
	);
	~Mesh();

	void render(Shader& shader, BasicCamera& camera) const;
	//GLuint getId() const;
};