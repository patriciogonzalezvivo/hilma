#include "hilma/ops/process.h"
#include "hilma/math.h"

#include <stdio.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/normal.hpp>

#define	RED_WEIGHT	    0.299
#define GREEN_WEIGHT	0.587
#define BLUE_WEIGHT	    0.114
#define INF             1E20

namespace hilma {

void sqrt(Image& _image) {
    int total = _image.width * _image.height * _image.channels;
    for (int i = 0; i < total; i++)
        _image.data[i] = std::sqrt(_image.data[i]);
}

void invert(Image& _image) {
    int total = _image.width * _image.height * _image.channels;
    for (int i = 0; i < total; i++)
        _image.data[i] = 1.0f -_image.data[i];
}

void gamma(Image& _image, float _gamma) {
    int total = _image.width * _image.height * _image.channels;
    for (int i = 0; i < total; i++)
        _image.data[i] = std::pow(_image.data[i], _gamma);
}

void autolevel(Image& _image){
    float lo = 1.0f;
    float hi = 0.0f;

    int total = _image.width * _image.height * _image.channels;
    for (int i = 0; i < total; i++) {
        float data = _image.data[i];
        lo = std::min(lo, data);
        hi = std::max(hi, data);
    }

    for (int i = 0; i < total; i++) {
        float data = _image.data[i];
        lo = std::min(lo, data);
        hi = std::max(hi, data);
    }

    if (hi == lo) {
        return;
    }

    for (int i = 0; i < total; i++)
        _image.data[i] =  (_image.data[i] - lo) / (hi - lo);
}

void flip(Image& _image) {
    const size_t stride = _image.width * _image.channels;
    float *row = (float*)malloc(stride * sizeof(float));
    float *low = &_image.data[0];
    float *high = &_image.data[(_image.height - 1) * stride];
    for (; low < high; low += stride, high -= stride) {
        std::memcpy(row, low, stride * sizeof(float));
        std::memcpy(low, high, stride * sizeof(float));
        std::memcpy(high, row, stride * sizeof(float));
    }
    free(row);
}

glm::vec2 getRange(const Image& _image) {
    float min =  10000.0f;
    float max = -10000.0f;

    int total = _image.width * _image.height * _image.channels;
    for (int i = 0; i < total; i++) {
        float val = _image.data[i];
        if (min > val) min = val;
        if (max < val) max = val;
    }

    return glm::vec2(min, max);
} 

void remap(Image& _image, float _in_min, float _int_max, float _out_min, float _out_max, bool _clamp) {
    int total = _image.width * _image.height * _image.channels;
    for (int i = 0; i < total; i++)
        _image.data[i] = remap(_image.data[i], _in_min, _int_max, _out_min, _out_max, _clamp);
} 

void threshold(Image& _image, float _threshold) {
    int total = _image.width * _image.height * _image.channels;
    for (int i = 0; i < total; i++)
        _image.data[i] = (_image.data[i] >= _threshold)? 1.0f : 0.0f;
}

/*
Copyright (C) 2006 Pedro Felzenszwalb ( https://cs.brown.edu/people/pfelzens/dt/ )

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

/* dt of 1d function using squared distance */
// static float *dt(float *f, int n) {
//     float *d = new float[n];
//     int *v = new int[n];
//     float *z = new float[n+1];
//     int k = 0;
//     v[0] = 0;
//     z[0] = -INF;
//     z[1] = +INF;
//     for (int q = 1; q <= n-1; q++) {
//         float s  = ((f[q] + square(q)) - (f[v[k]] + square(v[k]))) / (2 * q - 2*v[k]);
//         while (s <= z[k]) {
//             k--;
//             s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
//         }
//         k++;
//         v[k] = q;
//         z[k] = s;
//         z[k+1] = +INF;
//     }

//     k = 0;
//     for (int q = 0; q <= n-1; q++) {
//         while (z[k+1] < q)
//             k++;
//         d[q] = square(q-v[k]) + f[v[k]];
//     }

//     delete [] v;
//     delete [] z;
//     return d;
// }

static float *dt(float *f, int n) {
  float *d = new float[n];
  int *v = new int[n];
  float *z = new float[n+1];
  int k = 0;
  v[0] = 0;
  z[0] = -INF;
  z[1] = +INF;
  for (int q = 1; q <= n-1; q++) {
    float s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
    while (s <= z[k]) {
      k--;
      s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
    }
    k++;
    v[k] = q;
    z[k] = s;
    z[k+1] = +INF;
  }

  k = 0;
  for (int q = 0; q <= n-1; q++) {
    while (z[k+1] < q)
      k++;
    d[q] = square(q-v[k]) + f[v[k]];
  }

  delete [] v;
  delete [] z;
  return d;
}

/* dt of 2d function using squared distance */
void sdf(Image& _image) {
    if (_image.channels > 1) {
        std::cout << "We need a one channel image to compute an SDF" << std::endl;
        return;
    }

    int width = _image.width;
    int height = _image.height;
    float *f = new float[std::max(width, height)];

    // transform along columns
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++)
            f[y] = _image.data[y * width + x];
        
        float *d = dt(f, height);
        for (int y = 0; y < height; y++) 
            _image.data[y * width + x] = d[y];

        delete [] d;
    }

    // transform along rows
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++)
            f[x] = _image.data[y * width + x];
        
        float *d = dt(f, width);
        for (int x = 0; x < width; x++)
            _image.data[y * width + x] = d[x];
            
        delete [] d;
    }

    sqrt(_image);
    autolevel(_image);

    delete f;
}


/* dt of binary image using squared distance */
Image toSdf(const Image& _image, float _on) {
    int width = _image.getWidth();
    int height = _image.getHeight();
    Image rta = Image(width, height, 1);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if ( _image.getValue( _image.getIndex(x, y) ) == _on)
	            rta.setValue( rta.getIndex(x, y), 0.0f);
            else
                rta.setValue( rta.getIndex(x, y), INF);
        }
    }

    sdf(rta);
    return rta;
}

Image toNormalMap(const Image& _heightmap, float _zScale) {
    const int w = _heightmap.getWidth() - 1;
    const int h = _heightmap.getHeight() - 1;
    std::vector<glm::vec3> result(w * h);
    int i = 0;
    for (int y0 = 0; y0 < h; y0++) {
        const int y1 = y0 + 1;
        const float yc = y0 + 0.5f;
        for (int x0 = 0; x0 < w; x0++) {
            const int x1 = x0 + 1;
            const float xc = x0 + 0.5f;
            const float z00 = _heightmap.getValue( _heightmap.getIndex(x0, y0) ) * -_zScale;
            const float z01 = _heightmap.getValue( _heightmap.getIndex(x0, y1) ) * -_zScale;
            const float z10 = _heightmap.getValue( _heightmap.getIndex(x1, y0) ) * -_zScale;
            const float z11 = _heightmap.getValue( _heightmap.getIndex(x1, y1) ) * -_zScale;
            const float zc = (z00 + z01 + z10 + z11) / 4.f;
            const glm::vec3 p00(x0, y0, z00);
            const glm::vec3 p01(x0, y1, z01);
            const glm::vec3 p10(x1, y0, z10);
            const glm::vec3 p11(x1, y1, z11);
            const glm::vec3 pc(xc, yc, zc);
            const glm::vec3 n0 = glm::triangleNormal(pc, p00, p10);
            const glm::vec3 n1 = glm::triangleNormal(pc, p10, p11);
            const glm::vec3 n2 = glm::triangleNormal(pc, p11, p01);
            const glm::vec3 n3 = glm::triangleNormal(pc, p01, p00);
            result[i] = glm::normalize(n0 + n1 + n2 + n3) * 0.5f + 0.5f;
            i++;
        }
    }

    Image rta = Image(w, h, 3);
    rta.setColors(&result[0].r, result.size(), 3);
    return rta;
}

Image toLuma(const Image& _image) {
    int width = _image.getWidth();
    int height = _image.getHeight();

    Image rta = Image(width, height, 1);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            glm::vec4 c = _image.getColor( _image.getIndex(x, y) );
            float value = glm::dot(glm::vec3(c.x, c.y, c.z), glm::vec3(0.2126f, 0.7152f, 0.0722f));
            rta.setValue( rta.getIndex(x, y), value);
        }
    }

    return rta;
}

}