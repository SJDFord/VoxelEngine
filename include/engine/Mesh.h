#pragma once
#include <vector>
#include <memory>
#include <engine/Texture.h>
#include <engine/Vertex.h>

struct Mesh {
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<std::shared_ptr<Texture>> Textures;
};