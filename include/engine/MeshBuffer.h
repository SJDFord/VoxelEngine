#pragma once
#include <string>
#include <engine/Vertex.h>
#include <vector>
#include <engine/Shader.h>
#include <engine/BasicCamera.h>
#include <engine/Transform3.h>
#include <functional>
#include <engine/Material.h>
#include <memory>
#include <engine/Mesh.h>

class MeshBuffer
{
private:
	//std::vector<unsigned int> indices; // TODO: EBOs - see https://learnopengl.com/Getting-started/Hello-Triangle#:~:text=An%20EBO%20is%20a%20buffer,the%20solution%20to%20our%20problem.
	// Note also that for faces with different colours/textures we can't use EBOs
	size_t _vertexCount;
	const Material* _material;
	unsigned int _vao;
	unsigned int _vbo;
	
	MeshBuffer(const MeshBuffer&) = default;                // 1/5: Copy Ctor
	MeshBuffer(MeshBuffer&&) noexcept = default;            // 4/5: Move Ctor
	MeshBuffer& operator=(const MeshBuffer&) = default;     // 2/5: Copy Assignment
	MeshBuffer& operator=(MeshBuffer&&) noexcept = default; // 5/5: Move Assignment
public:
	// TODO: Revisit if Texture should be passed to Mesh and if rendering should be handled in this class
	// TODO: Maybe Texture should go into a Material class along with other properties like lighting maps
	// TODO: Maybe a mesh, transformation and material can be grouped together into a 'render object' with a render method accepting a camera and shader 
	MeshBuffer(
		Mesh mesh,
		const Material* material = nullptr // Optional material for the mesh (TODO: Multiple materials (for difference faces))
	);
	~MeshBuffer();

	size_t getVertexCount() const;
	const Material* getMaterial();

	void bind() const;
};