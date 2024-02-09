#include <Mesh.h>
// See https://learnopengl.com/Model-Loading/Mesh

Mesh::Mesh(
    const std::vector<Vertex> vertices, 
    const Material* material) {

    this->_vertices = vertices;
    this->_material = material;

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

    // Vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    if (this->_material) {
        fprintf(stdout, "Material enabled\n");
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        if (!this->_material->DiffuseMap) {
            throw "Material diffuse map not set";
        }
    }
    
    glBindVertexArray(0);

}

void Mesh::render(Shader& shader, BasicCamera& camera, std::vector<MeshTransformations> perInstanceTransformations, const std::function<void(const Shader&)>& setupShader) const {
    shader.use();
    if (this->_material) {
        // TODO: Select texture from texture list
        //unsigned int i = 0;
        //glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
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
        //shader.setInt("textureSampler", 0);
        shader.setInt("material.diffuse", 0);
        if (this->_material->SpecularMap) {
            shader.setInt("material.specular", 1);
        }
        if (this->_material->EmissionMap) {
            shader.setInt("material.emission", 2);
        }
        shader.setFloat("material.shininess", this->_material->Shininess);
        // TODO: Determine this value from texture channel(s)
        //shader.setBool("isGreyscale", true); // Needed for single channel images 
        // and finally bind the texture

        //this->_texture->bind();

        glActiveTexture(GL_TEXTURE0);
        this->_material->DiffuseMap->bind();
        
        if (this->_material->SpecularMap) {
            glActiveTexture(GL_TEXTURE1);
            this->_material->SpecularMap->bind();
        }


        if (this->_material->EmissionMap) {
            glActiveTexture(GL_TEXTURE2);
            this->_material->EmissionMap->bind();
        }
    }

    setupShader(shader);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)1600 / (float)1200, 0.1f, 100.0f);
    shader.setMat4("projection", projection);

    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("view", view);

    // TODO: Optimise this so we can just generate one mesh for all instances (one render call)
    // render mesh instances
    glBindVertexArray(_vao);
    for (unsigned int i = 0; i < perInstanceTransformations.size(); i++)
    {
        MeshTransformations transformations = perInstanceTransformations[i];
        
        glm::vec3 position = transformations.Position;
        float rotationAngle = transformations.RotationAngle;
        glm::vec3 rotationAxis = transformations.RotationAxis;
        glm::vec3 scaleFactor = transformations.ScaleFactor;

        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        // Scale, rotation then translation done in this order to minimize unwanted effects
        //fprintf(stdout, "Translating by %f, %f, %f, Rotating by %f, Scaling by %f, %f, %f\n", position.x, position.y, position.z, rotationAngle, scaleFactor.x, scaleFactor.y, scaleFactor.z);
        model = glm::translate(model, transformations.Position);
        model = glm::rotate(model, transformations.RotationAngle, transformations.RotationAxis);
        model = glm::scale(model, transformations.ScaleFactor);
   
        // glm::radians(angle)

        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    }
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    fprintf(stdout, "Mesh disposed: %s\n", "placeholder");
}