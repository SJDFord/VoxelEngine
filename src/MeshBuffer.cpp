#include <engine/MeshBuffer.h>
#include <engine/glCheck.h>
// See https://learnopengl.com/Model-Loading/Mesh

MeshBuffer::MeshBuffer(
    Mesh mesh, 
    const Material* material) {

    this->_vertexCount = mesh.Vertices.size();
    this->_material = material;

    // create buffers/arrays
    glCheck(glGenVertexArrays(1, &_vao));
    glCheck(glGenBuffers(1, &_vbo));

    glCheck(glBindVertexArray(_vao));
    // load data into vertex buffers
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glCheck(glBufferData(GL_ARRAY_BUFFER, _vertexCount * sizeof(Vertex), &mesh.Vertices[0], GL_STATIC_DRAW));

    // set the vertex attribute pointers
    // vertex Positions
    glCheck(glEnableVertexAttribArray(0));
    glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));

    // Vertex normals
    glCheck(glEnableVertexAttribArray(1));
    glCheck(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));

    if (this->_material) {
        fprintf(stdout, "Material enabled\n");
        // vertex texture coords
        glCheck(glEnableVertexAttribArray(2));
        glCheck(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));

        if (!(this->_material->DiffuseMap)) {
            throw "Material diffuse map not set";
        }
    }
    
    glCheck(glBindVertexArray(0));

}

size_t MeshBuffer::getVertexCount() const
{
    return _vertexCount;
}

const Material* MeshBuffer::getMaterial() {
    return _material;
}


void MeshBuffer::bind() const {
    glCheck(glBindVertexArray(_vao));
}

MeshBuffer::~MeshBuffer() {
    glCheck(glDeleteVertexArrays(1, &_vao));
    glCheck(glDeleteBuffers(1, &_vbo));
    fprintf(stdout, "Mesh buffer disposed: %s\n", "placeholder");
}