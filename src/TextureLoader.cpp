#include <engine/TextureLoader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

std::shared_ptr<Texture> TextureLoader::loadFromFile(const std::string& file, const std::string &type)
{
	int width, height, channelsInFile;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(file.c_str(), &width, &height, &channelsInFile, 0);

	if (!data) {
		throw "Failed to load texture";
	}
	
	std::string name = file;// file.substr(file.find_last_of('/') + 1);
	std::shared_ptr<Texture> texture = std::make_shared<Texture>(data, width, height, channelsInFile, type, name);

	stbi_image_free(data);
	return texture;
}