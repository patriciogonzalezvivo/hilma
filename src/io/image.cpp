
#include "hilma/io/png.h"
#include "hilma/io/hdr.h"
#include "hilma/math.h"

#include <iostream>
#include <stdio.h>
// #include <png.h>

#define STB_IMAGE_IMPLEMENTATION
#include "hilma/io/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "hilma/io/stb_image_write.h"

namespace hilma {

// LOAD
//
bool load(const std::string& _filename, Image& _image) {
    int width, height, channels;
    unsigned char* pixels = stbi_load(_filename.c_str(), &width, &height, &channels, 0);
    _image.deAllocate();
    _image.allocate(width, height, channels);
    int total = width * height * channels;
    for (int i = 0; i < total; i++)
        _image.setData(i, float(pixels[i]) / 255.0f );
    
    delete pixels;
    return true;
}


bool loadPng(const std::string& _filename, Image& _image) {
    return load(_filename, _image);
}

unsigned char* loadPng(const std::string& _filename, int* _width, int* _height, int* _channels) {
    return stbi_load(_filename.c_str(), _width, _height, _channels, 0);
}

bool loadHdr(const std::string& _filename, Image& _image ) {
    _image.deAllocate();
    _image.data = stbi_loadf(_filename.c_str(), &_image.width, &_image.height, &_image.channels, 0);
    return true;
}

float* loadHdr(const std::string& _filename, int* _width, int* _height, int* _channels) {
    return stbi_loadf(_filename.c_str(), _width, _height, _channels, 0);
}

// SAVE
//
bool saveHdr(const std::string& _filename, const float* _pixels, int _width, int _height, int _channels) {
    return stbi_write_hdr(_filename.c_str(), _width, _height, _channels, _pixels);
}

bool saveHdr(const std::string& _filename, Image& _image) {
    return stbi_write_hdr(_filename.c_str(), _image.width, _image.height, _image.channels, _image.data);
}

bool savePng(const std::string& _filename, const unsigned char* _pixels, int _width, int _height, int _channels) {
    return stbi_write_png(_filename.c_str(), _width, _height, _channels, _pixels, 0);
}

bool savePng(const std::string& _filename, Image& _image) {
    int total = _image.width * _image.height * _image.channels;
    unsigned char* pixels = new unsigned char[total];
    for (int i = 0; i < total; i++)
        pixels[i] = static_cast<char>(256 * clamp(_image.data[i], 0.0, 0.999));
    savePng(_filename, pixels, _image.width, _image.height, _image.channels);
    delete [] pixels;

    return true;
}


// bool savePng16(const std::string& _filename, uint16_t* _pixels, int _width, int _height, int _channels) {

//     /* create file */
//     FILE *fp = fopen(_filename.c_str(), "wb");
//     if (!fp){
//         std::runtime_error("[write_png_file] File could not be opened for writing");
//         return false;
//     }


//     /* initialize stuff */
//     png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

//     if (!png_ptr){
//         std::runtime_error("[write_png_file] png_create_write_struct failed");
//         return false;
//     }

//     png_infop info_ptr = png_create_info_struct(png_ptr);
//     if (!info_ptr){
//         std::runtime_error("[write_png_file] png_create_info_struct failed");
//         return false;
//     }

//     if (setjmp(png_jmpbuf(png_ptr))){
//         std::runtime_error("[write_png_file] Error during init_io");
//         return false;
//     }

//     png_init_io(png_ptr, fp);


//     /* write header */
//     if (setjmp(png_jmpbuf(png_ptr))){
//         std::runtime_error("[write_png_file] Error during writing header");
//         return false;
//     }

//     png_set_IHDR(png_ptr, info_ptr,
//             (png_uint_32) _width, (png_uint_32) _height,
//             (png_byte) 16, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
//             PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

//     png_write_info(png_ptr, info_ptr);


//     /* write bytes */
//     if (setjmp(png_jmpbuf(png_ptr))){
//         std::runtime_error("[write_png_file] Error during writing bytes");
//         return false;
//     }

//     /* Initialize rows of PNG. */
//     png_bytepp row_pointers = (png_bytepp) malloc (sizeof(png_bytep)*_height);
//     png_bytep row = (png_bytep) malloc(6 * _width * sizeof(png_byte));
//     for (size_t y = 0; y < _height; ++y) {
//         row_pointers[y] = (png_bytep) malloc (png_get_rowbytes(png_ptr,info_ptr));
//         for (size_t x = 0; x < _width; ++x) {
//             size_t i = y * _width + x;
//             png_save_uint_16(&row[x*6+0], (unsigned int)_pixels[i * _channels + 0] );
//             png_save_uint_16(&row[x*6+2], (unsigned int)_pixels[i * _channels + 1] );
//             png_save_uint_16(&row[x*6+4], (unsigned int)_pixels[i * _channels + 2] );
//         }
//         png_write_row(png_ptr, row);
//     }

//     /* end write */
//     if (setjmp(png_jmpbuf(png_ptr))){
//         std::runtime_error("[write_png_file] Error during end of write");
//         return false;
//     }

//     png_write_end(png_ptr, NULL);
//     fclose(fp);
//     free(row);

//     return true;
// }


}