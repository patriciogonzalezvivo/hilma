#include <iostream>
#include <stdio.h>
#include <cstring>

#include "hilma/types/Image.h"

namespace hilma {

Image::Image(): data(nullptr), width(0), height(0), channels(0) {
}

Image::Image(size_t _width, size_t _height, size_t _channels) {
    allocate(_width, _height, _channels);
}

Image::~Image() {
    deAllocate();
}

bool Image::allocate(size_t _width, size_t _height, size_t _channels) {
    width = _width;
    height = _height;
    channels = _channels;
    data = new float[_width * _height * _channels];
    return true;
}

void Image::deAllocate() {
    if (data)
        delete [] data;
}

float Image::getData(size_t _index) const {
    if (data == nullptr)
        return 0.0f;

    return data[_index];
}

glm::vec4 Image::getColor(size_t _index) const {
    glm::vec4 rta = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    if (data == nullptr)
        return rta;

    for (size_t i = 0; i < channels; i++)
        rta[i] = data[_index + i];

    return rta;
}

void Image::setData(size_t _index, float _data) {
    if (data == nullptr) {
        std::cout << "Data have been not pre allocated" << std::endl;
        return;
    }
    data[_index] = _data; 
}

void Image::setData(size_t _index, const float* _array1D, int _n) {
    if (data == nullptr) {
        std::cout << "Data have been not pre allocated" << std::endl;
        return;
    }

    // if (_n != channels) {
    //     std::cout << "The size of the data doesn't match the pre allocated space for it" << std::endl;
    //     return;
    // }

    // memcpy(&data[_index], _array1D, _n);

    for (size_t i = 0; i < _n; i++)
        data[_index + i] = _array1D[i];
}

}