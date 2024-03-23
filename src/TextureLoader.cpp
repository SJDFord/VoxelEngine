#include <engine/TextureLoader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include <engine/TexturePropertiesBuilder.h>

std::shared_ptr<Texture> TextureLoader::loadFromFile(const std::string& file, const TextureType& type)
{
	int width, height, channelsInFile;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(file.c_str(), &width, &height, &channelsInFile, 0);

	if (!data) {
		throw "Failed to load texture";
	}
	
	glm::uvec2 dimensions = { width, height };

	TextureFormat format;
	switch (channelsInFile) {
	case 1:
		format = GRAYSCALE;
		break;
	case 3:
		format = COLOUR;
		break;
	case 4:
		format = COLOUR_WITH_TRANSPARENCY;
		break;
	default:
		throw "Invalid format specified";
	}

	TexturePropertiesBuilder propertiesBuilder;
	TextureProperties properties = propertiesBuilder
		.setName(file)
		.setType(type)
		.setMipMaps(true)
		.build();

	std::shared_ptr<Texture> texture = std::make_shared<Texture>(data, dimensions, format, properties);

	stbi_image_free(data);
	return texture;
}