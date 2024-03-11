#pragma once

#include <memory>
#include <string>
#include <engine/Model.h>

class ModelLoader
{
public:
    Model loadFromFile(const std::string& file);
private:
    std::string _directory;
    std::vector<Mesh> _meshes;
    std::vector<std::shared_ptr<Texture>> _texturesLoaded; 

    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<std::shared_ptr<Texture>> ModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type);
};