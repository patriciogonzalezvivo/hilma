#include <iostream>
#include <stdio.h>
#include <cstring>

#include "hilma/types/Image.h"
#include "hilma/math.h"

namespace hilma {

Image::Image(): width(0), height(0), channels(0) {
}

Image::Image(const Image& _mother) {
    width = _mother.width;
    height = _mother.height;
    channels = _mother.channels;

    int total = width * height * channels;
    if (_mother.isAllocated()) {
        data.resize(total);
        std::memcpy(&data[0], &_mother.data[0], total * sizeof(float));
    }
}

Image::Image(int _width, int _height, int _channels) {
    allocate(_width, _height, _channels);
}

// bool Image::loadData(const uint8_t* _array3D, int _height, int _width, int _channels) {
Image::Image(const uint8_t* _array3D, int _height, int _width, int _channels) {
    set(_array3D, _height, _width, _channels);
}

Image::~Image() {
}

bool Image::allocate(size_t _width, size_t _height, size_t _channels) {
    width = _width;
    height = _height;
    channels = _channels;
    // data = new float[_width * _height * _channels];
    int total = _width * _height * _channels;

    if (total != data.size())
        data.resize(total);

    return true;
}

void Image::set(const uint8_t* _array3D, int _height, int _width, int _channels) {
    allocate(_width, _height, _channels);
    int total = width * height * channels;
    data.resize(total);
    for (int i = 0; i < total; i++)
        data[i] = _array3D[i]/255.0f;
}

void Image::setValue(size_t _index, float _data) {
    if (data.size() == 0) {
        std::cout << "Data have been not pre allocated" << std::endl;
        return;
    }
    data[_index] = _data; 
}

void Image::setValue(size_t _index, const float* _array1D, int _n) {
    if (data.size() == 0) {
        std::cout << "Data have been not pre allocated" << std::endl;
        return;
    }

    std::memcpy(&data[_index], _array1D, _n * sizeof(float));
}

void Image::setColors(const float* _array2D, int _m, int _n) {
    std::memcpy(&data[0], _array2D, _m * _n * sizeof(float));
}

// To numpy https://numpy.org/devdocs/reference/swig.interface-file.html
void Image::get(uint8_t **_array3D, int *_height, int *_width, int *_channels) {
    int total = width * height * channels;
    uint8_t * pixels = new uint8_t[total];
    for (int i = 0; i < total; i++)
        pixels[i] = static_cast<uint8_t>(256 * clamp(data[i], 0.0, 0.999));

    *_array3D = pixels;
    *_height = height;
    *_width = width;
    *_channels = channels;
}

float Image::getValue(size_t _index) const {
    if (data.size() == 0)
        return 0.0f;

    return data[_index];
}

glm::vec4 Image::getColor(size_t _index) const {
    glm::vec4 rta = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    if (data.size() == 0)
        return rta;

    for (size_t i = 0; i < channels; i++)
        rta[i] = data[_index + i];

    return rta;
}

}