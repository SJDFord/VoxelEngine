#pragma once
#include <memory>
#include <engine/Shader.h>
#include <engine/Mesh.h>
#include <engine/Texture.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include "MeshBuffer.h"
#include "MeshRenderer.h"
#include "TextureLoader.h"
#include <iostream>

struct Model 
{
    std::vector<Mesh> Meshes;
};