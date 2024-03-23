#include <engine/TexturePropertiesBuilder.h>

TexturePropertiesBuilder& TexturePropertiesBuilder::setName(std::string name) {
    _name = name;    
    return *this;
}


TexturePropertiesBuilder& TexturePropertiesBuilder::setType(TextureType type) {
    _type = type;
    return *this;
}

TexturePropertiesBuilder& TexturePropertiesBuilder::setWrapMode(TextureWrapMode horizontal, TextureWrapMode vertical) {
    _wrapping = { horizontal, vertical };
    return *this;
}
TexturePropertiesBuilder& TexturePropertiesBuilder::setWrapMode(TextureWrapMode wrapMode) {
    setWrapMode(wrapMode, wrapMode);
    return *this;
}

TexturePropertiesBuilder& TexturePropertiesBuilder::setFilter(TextureFilter min, TextureFilter mag) {
    _filtering = { min, mag };
    return *this;
}

TexturePropertiesBuilder& TexturePropertiesBuilder::setFilter(TextureFilter filter) {
    setFilter(filter, filter);
    return *this;
}

TexturePropertiesBuilder& TexturePropertiesBuilder::setMipMaps(bool useMipMaps) {
    _useMipMaps = useMipMaps;
        
    return *this;
}

TextureProperties TexturePropertiesBuilder::build() {
    return {
        _name,
	    _type,
        _wrapping,
        _filtering,
        _useMipMaps
    };
}