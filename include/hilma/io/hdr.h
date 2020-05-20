#pragma once

#include <string>
#include "hilma/types/Image.h"

namespace hilma {

bool    loadHdr(const std::string& _filename, Image& _image);
float*  loadHdr(const std::string& _filename, int* _width, int* _height, int* _channels);

bool    saveHdr(const std::string& _filename, Image& _image);
bool    saveHdr(const std::string& _filename, const float* _pixels, int _width, int _height, int _channels);

}
