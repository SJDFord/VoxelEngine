#include <ShaderLoader.h>
#include <fstream>
#include <sstream>
#include <iostream>

std::shared_ptr<Shader> ShaderLoader::loadFromFile(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) const {
    std::string vertexCode;
    std::string fragmentCode;
    
    try
    {
        vertexCode = readFileAsString(vertexShaderFile);
        fragmentCode = readFileAsString(fragmentShaderFile);
    }
    catch (std::ifstream::failure& e)
    {
        throw strcat("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: ", e.what());
    }

    return std::make_shared<Shader>(vertexCode, fragmentCode);
}

std::string ShaderLoader::readFileAsString(const std::string& file) const {
    std::string fileContent;
    std::ifstream fileStream;
    // ensure ifstream objects can throw exceptions:
    fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    fileStream.open(file);
    std::stringstream fileStringStream;
    // read file's buffer contents into streams
    fileStringStream << fileStream.rdbuf();
    // close file handlers
    fileStream.close();
    // convert stream into string
    fileContent = fileStringStream.str();
    return fileContent;
}