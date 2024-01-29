#include <Mesh.h>
// See https://learnopengl.com/Model-Loading/Mesh

Mesh::Mesh(const std::vector<Vertex> vertices, const Texture *texture, const std::vector<glm::vec3> positions) {
    this->_vertices = vertices;
    this->_texture = texture;
    this->_positions = positions;


    // create buffers/arrays
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);

}

void Mesh::render(Shader& shader, BasicCamera& camera) const {
    // TODO: Select texture from texture list
    unsigned int i = 0;
    glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
    // retrieve texture number (the N in diffuse_textureN)
    /*
    string number;
    string name = textures[i].type;
    if (name == "texture_diffuse")
        number = std::to_string(diffuseNr++);
    else if (name == "texture_specular")
        number = std::to_string(specularNr++); // transfer unsigned int to string
    else if (name == "texture_normal")
        number = std::to_string(normalNr++); // transfer unsigned int to string
    else if (name == "texture_height")
        number = std::to_string(heightNr++); // transfer unsigned int to string
    */
    // now set the sampler to the correct texture unit
    //glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
    shader.setInt("textureSampler", 0);
    shader.setBool("isGreyscale", true); // Needed for single channel images 
    // and finally bind the texture
   
    this->_texture->bind();
    shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)1600 / (float)1200, 0.1f, 100.0f);
    shader.setMat4("projection", projection);

    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("view", view);

    // render boxes
    glBindVertexArray(_vao);
    for (unsigned int i = 0; i < _positions.size(); i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, _positions[i]);
        float angle = 0;
        //float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        // TODO: scale model (if required)

        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    }
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    fprintf(stdout, "Mesh disposed: %s\n", "placeholder");
}