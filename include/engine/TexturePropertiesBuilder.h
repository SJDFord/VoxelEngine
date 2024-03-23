#include <glm/glm.hpp>
#include <engine/Texture.h>

class TexturePropertiesBuilder {
private:
	std::string _name = "";
    TextureType _type = TextureType::DIFFUSE;
    TextureWrapping _wrapping = { TextureWrapMode::REPEAT, TextureWrapMode::REPEAT };
    TextureFiltering _filtering = { TextureFilter::NEAREST, TextureFilter::NEAREST };
    bool _useMipMaps = false;
public:
	TexturePropertiesBuilder& setName(std::string name);
    TexturePropertiesBuilder& setType(TextureType type);
    TexturePropertiesBuilder& setWrapMode(TextureWrapMode horizontal, TextureWrapMode vertical);
    TexturePropertiesBuilder& setWrapMode(TextureWrapMode wrapMode);
    TexturePropertiesBuilder& setFilter(TextureFilter min, TextureFilter mag);
    TexturePropertiesBuilder& setFilter(TextureFilter filter);
    TexturePropertiesBuilder& setMipMaps(bool useMipMaps);
	TextureProperties build();
};