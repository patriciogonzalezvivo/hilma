#pragma once

#include <string>
#include "hilma/types/image.h"
#include "hilma/io/auto.h"

namespace hilma {

float*  loadHdr(const std::string& _filename, int* _width, int* _height, int* _channels);

inline bool     loadHdr(const std::string& _filename, Image& _image, int _channels = 0) {
    return load(_filename, _image, _channels);
}

inline Image    loadHdr(const std::string& _filename, int _channel = 0) {
    Image out;
    load(_filename, out, _channel);
    return out;
}

bool    saveHdr(const std::string& _filename, const Image& _image);
bool    saveHdr(const std::string& _filename, const float* _pixels, int _width, int _height, int _channels);

}
