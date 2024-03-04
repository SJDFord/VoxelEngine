#pragma once
#include <vector>
#include <engine/Vertex.h>

struct Mesh {
	std::vector<Vertex> Vertices;
	//std::vector<unsigned int> indices; // TODO: EBOs - see https://learnopengl.com/Getting-started/Hello-Triangle#:~:text=An%20EBO%20is%20a%20buffer,the%20solution%20to%20our%20problem.
};