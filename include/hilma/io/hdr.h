#pragma once

#include <string>

namespace hilma {

float*  loadHdr(const std::string& _filename, int* _width, int* _height, int* _channels);
bool    saveHdr(const std::string& _filename, const float* _pixels, int _width, int _height, int _channels);

}
