#pragma once

#include "hilma/types/Image.h"

namespace hilma {

void    sqrt(Image& _image);
void    invert(Image& _image);
void    gamma(Image& _image, float _gamma);
void    autolevel(Image& _image);
void    flip(Image& _image);
void    remap(Image& _image, float _in_min, float _int_max, float _out_min, float _out_max, bool _clamp);
glm::vec2 getRange(const Image& _image);

void    threshold(Image& _image, float _threshold = 0.5f);

Image   toNormalMap(const Image& _heightmap, float _zScale = 10.0f);
Image   toLuma(const Image& _image);

void    sdf(Image& _image);
Image   toSdf(const Image& _image, float _on = 1.0f);

Image   denoise(const Image& _color, const Image& _normal, const Image& _albedo, bool _hdr = true);

}