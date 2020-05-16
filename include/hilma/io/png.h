#pragma once

#include <string>

namespace hilma {

unsigned char*  loadPng(const std::string& _filename, int* _width, int* _height, int* _channels);
bool            savePng(const std::string& _filename, const unsigned char* _pixels, int _width, int _height, int _channels);

// bool savePng16(const std::string& _filename, uint16_t* _pixels, int _width, int _height, int _channels);

}
