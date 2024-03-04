#include <engine/Shader.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <engine/glCheck.h>

// TODO: Use uniform buffer object (UBO) instead of multiple uniform calls
Shader::Shader(const std::string& vertexCode, const std::string& fragmentCode) {  
    unsigned int vertex, fragment;
    vertex = compile(vertexCode, GL_VERTEX_SHADER);
    fragment = compile(fragmentCode, GL_FRAGMENT_SHADER);
    
    // shader Program
    glCheck(this->_id = glCreateProgram());
    glCheck(glAttachShader(this->_id, vertex));
    glCheck(glAttachShader(this->_id, fragment));
    glCheck(glLinkProgram(this->_id));
    checkCompileErrors(this->_id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glCheck(glDeleteShader(vertex));
    glCheck(glDeleteShader(fragment));
}

void Shader::use() const
{
    glCheck(glUseProgram(this->_id));
}

void Shader::setBool(const std::string& name, bool value) const
{
    glCheck(glUniform1i(getUniformLocation(name), (int)value));
}

void Shader::setInt(const std::string& name, int value) const
{
    glCheck(glUniform1i(getUniformLocation(name), value));
}

void Shader::setFloat(const std::string& name, float value) const
{
    glCheck(glUniform1f(getUniformLocation(name), value));
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glCheck(glUniform2fv(getUniformLocation(name), 1, &value[0]));
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
    glCheck(glUniform2f(getUniformLocation(name), x, y));
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glCheck(glUniform3fv(getUniformLocation(name), 1, &value[0]));
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glCheck(glUniform3f(getUniformLocation(name), x, y, z));
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glCheck(glUniform4fv(getUniformLocation(name), 1, &value[0]));
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glCheck(glUniform4f(getUniformLocation(name), x, y, z, w));
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glCheck(glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glCheck(glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glCheck(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

GLuint Shader::getId() const
{
    return this->_id;
}

GLuint Shader::compile(const std::string& code, GLenum type) const
{
    unsigned int shader;
    glCheck(shader = glCreateShader(type));

    const char* codeStr = code.c_str();
    glCheck(glShaderSource(shader, 1, &codeStr, NULL));
    glCheck(glCompileShader(shader));
    checkCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return shader;
}

void Shader::checkCompileErrors(GLuint shader, std::string type) const
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            printf("Error (%s)", infoLog);
            throw strcat(strcat("ERROR::SHADER_COMPILATION_ERROR of type: ", type.c_str()), infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);

            throw strcat(strcat("ERROR::PROGRAM_LINKING_ERROR of type: ", type.c_str()), infoLog);
       }
    }
}

GLuint Shader::getUniformLocation(const std::string& name) const
{
    unsigned int location;
    glCheck(location = glGetUniformLocation(this->_id, name.c_str()));
    return location;
}

Shader::~Shader() {
    glCheck(glDeleteProgram(this->_id));
    fprintf(stdout, "Shader disposed: %s\n", "placeholder");
}