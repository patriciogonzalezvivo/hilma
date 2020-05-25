#pragma once

#include <string>
#include "hilma/types/Image.h"

namespace hilma {

Image           loadJpg(const std::string& _filename, int _channels = 0);
bool            loadJpg(const std::string& _filename, Image& _image, int _channels = 0);
unsigned char*  loadJpg(const std::string& _filename, int* _width, int* _height, int* _channels);

bool            saveJpg(const std::string& _filename, Image& _image);
bool            saveJpg(const std::string& _filename, const unsigned char* _pixels, int _width, int _height, int _channels);

}
