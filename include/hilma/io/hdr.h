#pragma once

#include <string>
#include "hilma/types/Image.h"

namespace hilma {

Image   loadHdr(const std::string& _filename, int _channels = 0);
bool    loadHdr(const std::string& _filename, Image& _image, int _channels = 0);
float*  loadHdr(const std::string& _filename, int* _width, int* _height, int* _channels);

bool    saveHdr(const std::string& _filename, Image& _image);
bool    saveHdr(const std::string& _filename, const float* _pixels, int _width, int _height, int _channels);

}
