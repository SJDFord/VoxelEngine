#pragma once
#include <Shader.h>
#include <string>
#include <memory>

class ShaderLoader {
private:
	// TODO: Move into a more generic File IO location as this is not shader specific
	std::string readFileAsString(const std::string& file) const;
public:
	std::shared_ptr<Shader> loadFromFile(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) const;
};
