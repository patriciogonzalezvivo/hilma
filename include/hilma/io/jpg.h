#pragma once

#include <string>
#include "hilma/types/Image.h"
#include "hilma/io/auto.h"

namespace hilma {

inline bool     loadJpg(const std::string& _filename, Image& _image, int _channels = 0) {
    return load(_filename, _image, _channels);
}

inline Image    loadJpg(const std::string& _filename, int _channels = 0) {
    Image out;
    load(_filename, out, _channels);
    return out;
}

inline unsigned char* loadJpg(const std::string& _filename, int* _width, int* _height, int* _channels) {
    return load(_filename.c_str(), _width, _height, _channels);
}

bool            saveJpg(const std::string& _filename, Image& _image);
bool            saveJpg(const std::string& _filename, const unsigned char* _pixels, int _width, int _height, int _channels);

}
