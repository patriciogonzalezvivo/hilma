#pragma once

#include <string>

#include "hilma/types/mesh.h"
#include "hilma/types/image.h"

namespace hilma {

bool            load(const std::string& _filename, Image& _image, int _channels = 0);
unsigned char*  load(const std::string& _filename, int* _width, int* _height, int* _channels);

// inline Image    load(const std::string& _filename, int _channels = 0) {
//     Image out;
//     load(_filename, out, _channels);
//     return out;
// }

bool            load(const std::string& _filename, Mesh& _mesh);

bool            save(const std::string& _filename, const Image& _image);

}
