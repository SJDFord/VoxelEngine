#pragma once

#include <memory>
#include <string>
#include <engine/Texture.h>

class TextureLoader
{
public:
	std::shared_ptr<Texture> loadFromFile(const std::string& file);
};