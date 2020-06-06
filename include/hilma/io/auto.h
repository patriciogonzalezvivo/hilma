#pragma once

#include <string>

#include "hilma/types/Image.h"

namespace hilma {

bool            load(const std::string& _filename, Image& _image, int _channels = 0);
unsigned char*  load(const std::string& _filename, int* _width, int* _height, int* _channels);
inline Image    load(const std::string& _filename, int _channels = 0) {
    Image out;
    load(_filename, out, _channels);
    return out;
}

bool            save(const std::string& _filename, Image& _image);

}
