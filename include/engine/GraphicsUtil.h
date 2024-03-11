#pragma once
#include <glm/glm.hpp>
#include <engine/Mesh.h>
#include <engine/Model.h>


std::shared_ptr<MeshBuffer> createMeshBuffer(const Mesh& mesh) {
    return std::make_shared<MeshBuffer>(mesh);
}

std::vector<std::shared_ptr<MeshBuffer>> createMeshBuffers(const std::vector<Mesh>& meshes) {
    std::vector<std::shared_ptr<MeshBuffer>> meshBuffers;
    for (int i = 0; i < meshes.size(); i++) {
        Mesh m = meshes[i];
        meshBuffers.push_back(createMeshBuffer(m));
    }
    return meshBuffers;
}

std::vector<std::shared_ptr<MeshBuffer>> createMeshBuffers(const Model& model) {
    return createMeshBuffers(model.Meshes);
}

struct TextureCoords {
    static constexpr glm::vec2 BottomLeft = glm::vec2(0.0f, 0.0f);
    static constexpr glm::vec2 TopLeft = glm::vec2(0.0f, 1.0f);
    static constexpr glm::vec2 BottomRight = glm::vec2(1.0f, 0.0f);
    static constexpr glm::vec2 TopRight = glm::vec2(1.0f, 1.0f);
};

struct CubeCoords {
    static constexpr glm::vec3 FrontBottomLeft = glm::vec3(-0.5f, -0.5f, 0.5f);
    static constexpr glm::vec3 FrontTopLeft = glm::vec3(-0.5f, 0.5f, 0.5f);
    static constexpr glm::vec3 FrontBottomRight = glm::vec3(0.5f, -0.5f, 0.5f);
    static constexpr glm::vec3 FrontTopRight = glm::vec3(0.5f, 0.5f, 0.5f);
    
    static constexpr glm::vec3 BackBottomLeft = glm::vec3(-0.5f, -0.5f, -0.5f);
    static constexpr glm::vec3 BackTopLeft = glm::vec3(-0.5f, 0.5f, -0.5f);
    static constexpr glm::vec3 BackBottomRight = glm::vec3(0.5f, -0.5f, -0.5f);
    static constexpr glm::vec3 BackTopRight = glm::vec3(0.5f, 0.5f, -0.5f);
};

struct CubeNormals {

    static constexpr glm::vec3 Back = glm::vec3(0.0f, 0.0f, -1.0f);
    static constexpr glm::vec3 Front = glm::vec3(0.0f, 0.0f, 1.0f);
    static constexpr glm::vec3 Left = glm::vec3(-1.0f, 0.0f, 0.0f);
    static constexpr glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
    static constexpr glm::vec3 Bottom = glm::vec3(0.0f, -1.0f, 0.0f);
    static constexpr glm::vec3 Top = glm::vec3(0.0f, 1.0f, 0.0f);
};

struct Meshes {
    static Mesh Cube;
};

Mesh Meshes::Cube = {
        {
            // Back
            {CubeCoords::BackBottomLeft, CubeNormals::Back, TextureCoords::BottomRight},
            {CubeCoords::BackTopRight, CubeNormals::Back, TextureCoords::TopLeft},
            {CubeCoords::BackBottomRight, CubeNormals::Back, TextureCoords::BottomLeft},

            {CubeCoords::BackTopRight, CubeNormals::Back, TextureCoords::TopLeft},
            {CubeCoords::BackBottomLeft, CubeNormals::Back, TextureCoords::BottomRight},
            {CubeCoords::BackTopLeft, CubeNormals::Back, TextureCoords::TopRight},
            

            // Front
            {CubeCoords::FrontBottomLeft, CubeNormals::Front, TextureCoords::BottomLeft},
            {CubeCoords::FrontBottomRight, CubeNormals::Front, TextureCoords::BottomRight},
            {CubeCoords::FrontTopRight, CubeNormals::Front, TextureCoords::TopRight},

            {CubeCoords::FrontTopRight, CubeNormals::Front, TextureCoords::TopRight},
            {CubeCoords::FrontTopLeft, CubeNormals::Front, TextureCoords::TopLeft},
            {CubeCoords::FrontBottomLeft, CubeNormals::Front,TextureCoords::BottomLeft},

            // Left
            {CubeCoords::FrontTopLeft, CubeNormals::Left, TextureCoords::TopRight},
            {CubeCoords::BackTopLeft,  CubeNormals::Left, TextureCoords::TopLeft},
            {CubeCoords::BackBottomLeft,  CubeNormals::Left, TextureCoords::BottomLeft},

            {CubeCoords::BackBottomLeft, CubeNormals::Left, TextureCoords::BottomLeft},
            {CubeCoords::FrontBottomLeft, CubeNormals::Left, TextureCoords::BottomRight},
            {CubeCoords::FrontTopLeft, CubeNormals::Left, TextureCoords::TopRight},

            // Right
            {CubeCoords::FrontTopRight, CubeNormals::Right, TextureCoords::TopLeft},
            {CubeCoords::BackBottomRight, CubeNormals::Right, TextureCoords::BottomRight},
            {CubeCoords::BackTopRight, CubeNormals::Right, TextureCoords::TopRight},

            {CubeCoords::BackBottomRight, CubeNormals::Right, TextureCoords::BottomRight},
            {CubeCoords::FrontTopRight, CubeNormals::Right, TextureCoords::TopLeft},
            {CubeCoords::FrontBottomRight, CubeNormals::Right, TextureCoords::BottomLeft},

            // Bottom
            {CubeCoords::BackBottomLeft, CubeNormals::Bottom, TextureCoords::BottomLeft},
            {CubeCoords::BackBottomRight, CubeNormals::Bottom, TextureCoords::BottomRight},
            {CubeCoords::FrontBottomRight, CubeNormals::Bottom, TextureCoords::TopRight},

            {CubeCoords::FrontBottomRight, CubeNormals::Bottom, TextureCoords::TopRight},
            {CubeCoords::FrontBottomLeft, CubeNormals::Bottom, TextureCoords::TopLeft},
            {CubeCoords::BackBottomLeft, CubeNormals::Bottom,TextureCoords::BottomLeft},

            // Top
            {CubeCoords::BackTopLeft, CubeNormals::Top, TextureCoords::TopLeft},
            {CubeCoords::FrontTopRight, CubeNormals::Top, TextureCoords::BottomRight},
            {CubeCoords::BackTopRight, CubeNormals::Top, TextureCoords::TopRight},

            {CubeCoords::FrontTopRight, CubeNormals::Top, TextureCoords::BottomRight},
            {CubeCoords::BackTopLeft, CubeNormals::Top, TextureCoords::TopLeft},
            {CubeCoords::FrontTopLeft, CubeNormals::Top, TextureCoords::BottomLeft},
        },
        {},
        {}
    };