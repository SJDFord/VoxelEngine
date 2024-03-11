#include <engine/ModelLoader.h>
#include <algorithm>

Model ModelLoader::loadFromFile(const std::string& file) {
    this->_texturesLoaded.clear();
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        throw "ERROR::ASSIMP:: " + std::string(importer.GetErrorString());
    }
    // retrieve the directory path of the filepath
    _directory = file.substr(0, file.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);

    Model model = {_meshes};
    fprintf(stdout, "Loaded model: %s (%i meshes) (%i textures)\n", file.c_str(), _meshes.size(), _texturesLoaded.size());
    return model;
}

void ModelLoader::processNode(aiNode *node, const aiScene *scene) {
    // process each mesh located at the current node
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            _meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
}


Mesh ModelLoader::processMesh(aiMesh *mesh, const aiScene *scene) {
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    // walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            //fprintf(stdout, "Setting normals: %f %f %f\n", vector.x, vector.y, vector.z);
        }
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            /*
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
            */

            //fprintf(stdout, "Setting texture coords: %f, %f\n", vec.x, vec.y);
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);        
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    /*
    // 3. normal maps
    std::vector<std::shared_ptr<Texture>> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<std::shared_ptr<Texture>> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    */

    // return a mesh object created from the extracted mesh data
    
    Mesh m = {vertices, indices, textures};
    return m;
}

std::vector<std::shared_ptr<Texture>> ModelLoader::loadMaterialTextures(aiMaterial *mat, aiTextureType type) {
    TextureType engineTextureType;
    switch (type) {
    case aiTextureType_DIFFUSE:
        engineTextureType = TextureType::DIFFUSE;
        break;
    case aiTextureType_SPECULAR:
        engineTextureType = TextureType::SPECULAR;
        break;
    case aiTextureType_EMISSIVE:
        engineTextureType = TextureType::EMISSIVE;
        break;
    default:
        throw "Assimp texture type not supported";
    }
    
    std::vector<std::shared_ptr<Texture>> meshTextures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string textureName = this->_directory + "/" + str.C_Str();
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        auto nameMatches = [textureName](std::shared_ptr<Texture> texture) { return texture->getName().compare(textureName) == 0; };
        auto loadedTexture = std::find_if(std::begin(_texturesLoaded), std::end(_texturesLoaded), nameMatches);
        if (loadedTexture == std::end(_texturesLoaded)) {
            TextureLoader textureLoader;
            std::shared_ptr<Texture> texture = textureLoader.loadFromFile(textureName, engineTextureType);

            meshTextures.push_back(texture);
            _texturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
        else {
            meshTextures.push_back(*loadedTexture);
        }
    }
    return meshTextures;
}