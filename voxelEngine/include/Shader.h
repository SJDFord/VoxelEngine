#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

class Shader
{
private:

	GLuint _id;

	Shader(const Shader&) = default;                // 1/5: Copy Ctor
	Shader(Shader&&) noexcept = default;            // 4/5: Move Ctor
	Shader& operator=(const Shader&) = default;     // 2/5: Copy Assignment
	Shader& operator=(Shader&&) noexcept = default; // 5/5: Move Assignment

    void checkCompileErrors(GLuint shader, std::string type) const;
public:
	Shader(const std::string& vertexCode, const std::string& fragmentCode);
	~Shader();

    void use() const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    
	GLuint getId() const;
};