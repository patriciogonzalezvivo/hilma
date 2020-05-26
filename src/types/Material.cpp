#include "hilma/types/Material.h"

#include "hilma/io/jpg.h"
#include "hilma/io/png.h"
#include "hilma/io/hdr.h"
#include "hilma/fs.h"

namespace hilma {

Material::Material(const std::string& _name): illuminationModel(0), name(_name) {
}

void Material::set(const std::string& _property, const std::string& _filename) {
    texturesPaths[_property] = _filename;

    if ( urlExists(_filename) ) {
        ImagePtr image = std::make_shared<Image>( Image() );
        std::string ext = getExt(_filename);
        bool loaded = false;

        if (ext == "jpg" || ext == "JPG" || 
            ext == "jpeg" || ext == "JPEG" )
            loaded = loadJpg(_filename, *image);
        else if (ext == "png" || ext == "PNG")
            loaded = loadPng(_filename, *image);
        else if (ext == "hdr" || ext == "HDR")
            loaded = loadHdr(_filename, *image);
        // TODO:
        //      - add more image formats

        if (loaded) {
            textures[_property] = image;
            properties[_property] = TEXTURE;
        }
    }
}

void Material::set(const std::string& _property, const float* _array1D, int _n) {
    colors[_property] = glm::vec3(_array1D[0], _array1D[1], _array1D[2]);
    properties[_property] = COLOR;
}

void Material::set(const std::string& _property, const glm::vec3& _color) {
    colors[_property] = _color;
    properties[_property] = COLOR;
}

void Material::set(const std::string& _property, const float _value) {
    values[_property] = _value;
    properties[_property] = VALUE;
}

std::string Material::getImagePath(const std::string& _property) const {
    const std::map<const std::string, std::string>::const_iterator it = texturesPaths.find(_property);
    if (it != texturesPaths.end() )
        return it->second;

    return "";
}

glm::vec3 Material::getColor(const std::string& _property, const glm::vec2& _uv) const {
    if (haveProperty(_property)) {
        MaterialPropertyType type = properties.find(_property)->second;
        if (type == TEXTURE) {
            const ImagePtr tex = textures.find(_property)->second;
            return tex->getColor( tex->getIndexUV(_uv.x, _uv.y) );
        }
        else if (type == COLOR)
            return colors.find(_property)->second;
        else if (type == VALUE)
            return glm::vec3(values.find(_property)->second);
    }

    return glm::vec3(-1.0);
}

glm::vec3 Material::getColor(const std::string& _property) const {
    if (haveProperty(_property)) {
        MaterialPropertyType type = properties.find(_property)->second;
        if (type == COLOR)
            return colors.find(_property)->second;
        else if (type == VALUE)
            return glm::vec3(values.find(_property)->second);
    }

    return glm::vec3(-1.0);
}

float Material::getValue(const std::string& _property, const glm::vec2& _uv) const {
    if (haveProperty(_property)) {
        MaterialPropertyType type = properties.find(_property)->second;
        if (type == TEXTURE) {
            const ImagePtr tex = textures.find(_property)->second;
            return tex->getColor( tex->getIndexUV( _uv.x, _uv.y ) )[0];
        }
        else if (type == COLOR)
            return colors.find(_property)->second.x;
        else if (type == VALUE)
            return values.find(_property)->second;
    }

    return -1.0;
}

float Material::getValue(const std::string& _property) const {
    if (haveProperty(_property)) {
        MaterialPropertyType type = properties.find(_property)->second;
        if (type == COLOR)
            return colors.find(_property)->second.r;
        else if (type == VALUE)
            return values.find(_property)->second;
    }

    return -1.0;
}

bool Material::haveProperty(const std::string& _property) const {
    return properties.find(_property) != properties.end();
}

}