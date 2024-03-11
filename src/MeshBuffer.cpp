#include <engine/MeshBuffer.h>
#include <engine/glCheck.h>
// See https://learnopengl.com/Model-Loading/Mesh

MeshBuffer::MeshBuffer(
    Mesh mesh) {

    this->_vertexCount = mesh.Vertices.size();
    this->_indexCount = mesh.Indices.size();
    this->_textures = mesh.Textures;

    // create buffers/arrays
    glCheck(glGenVertexArrays(1, &_vao));
    glCheck(glGenBuffers(1, &_vbo));
    if (mesh.Indices.size() > 0) {
        glCheck(glGenBuffers(1, &_ebo));
    }

    glCheck(glBindVertexArray(_vao));
    // load data into vertex buffers
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glCheck(glBufferData(GL_ARRAY_BUFFER, _vertexCount * sizeof(Vertex), &mesh.Vertices[0], GL_STATIC_DRAW));
    

    if (mesh.Indices.size() > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.Indices.size() * sizeof(unsigned int),
            &mesh.Indices[0], GL_STATIC_DRAW);

    }    

    // set the vertex attribute pointers
    // vertex Positions
    glCheck(glEnableVertexAttribArray(0));
    glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));

    // Vertex normals
    glCheck(glEnableVertexAttribArray(1));
    glCheck(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));

    if (mesh.Textures.size() > 0) {
        // vertex texture coords
        glCheck(glEnableVertexAttribArray(2));
        glCheck(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));
    }
    
    glCheck(glBindVertexArray(0));

}


std::vector<std::shared_ptr<Texture>> MeshBuffer::getTextures() const {
    return _textures;
}

void MeshBuffer::draw() const {
    bool wireframeMode = false;
    if (wireframeMode) {
        glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    }
    glCheck(glBindVertexArray(_vao));
    if (_indexCount > 0) {
        glCheck(glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0));
    }
    else {
        glCheck(glDrawArrays(GL_TRIANGLES, 0, _vertexCount));
    }
    glCheck(glBindVertexArray(0));
    if (wireframeMode) {
        glCheck(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    }
}

MeshBuffer::~MeshBuffer() {
    glCheck(glDeleteVertexArrays(1, &_vao));
    glCheck(glDeleteBuffers(1, &_vbo));
    glCheck(glDeleteBuffers(1, &_ebo));
}