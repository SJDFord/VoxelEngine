#pragma once

#include <memory>
#include <string>
#include <Texture.h>

class TextureLoader
{
public:
	std::shared_ptr<Texture> loadFromFile(const std::string& file);
};