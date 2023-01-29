#pragma once

#include <string>
#include "hilma/types/image.h"
#include "hilma/io/auto.h"

namespace hilma {


inline bool loadPng(const std::string& _filename, Image& _image, int _channels = 0) {
    return load(_filename, _image, _channels);
}

inline Image loadPng(const std::string& _filename, int _channels = 0) {
    Image out;
    load(_filename, out, _channels);
    return out;
}

inline unsigned char* loadPng(const std::string& _filename, int* _width, int* _height, int* _channels) {
    return load(_filename.c_str(), _width, _height, _channels);
}

bool            savePng(const std::string& _filename, const Image& _image);
bool            savePng(const std::string& _filename, const unsigned char* _pixels, int _width, int _height, int _channels);

// bool savePng16(const std::string& _filename, uint16_t* _pixels, int _width, int _height, int _channels);
}
