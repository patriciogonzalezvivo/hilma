#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Ray.h"
#include "hilma/types/Mesh.h"
#include "hilma/io/png.h"

using namespace hilma;
using namespace glm;

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

float hit_sphere(const point3& center, float radius, const Ray& r) {
    vec3 oc = r.getOrigin() - center;
    float a = dot(r.getDirection(), r.getDirection());
    float half_b = dot(oc, r.getDirection());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}

color ray_color(const Ray& r) {
    float t = hit_sphere(point3(0,0,-1), 0.5, r);
    if (t > 0.0) {
        vec3 N = normalize( vec3(r.getAt(t)) - vec3(0.0f, 0.0f, -1.0f));
        return 0.5f * color(N.x + 1.0, N.y + 1.0, N.z + 1.0);
    }

    vec3 unit_direction = normalize(r.getDirection());
    t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f-t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
}

int main(int argc, char **argv) {

    const float aspect_ratio = 16.0f / 9.0f;
    const int image_width = 1024;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    point3 origin(0.0f, 0.0f, 0.0f);
    vec3 horizontal(4.0f, 0.0f, 0.0f);
    vec3 vertical(0.0f, 2.25f, 0.0f);
    
    point3 lower_left_corner = origin - horizontal * 0.5f - vertical * 0.5f - vec3(0.0f,0.0f,1.0f);
    
    unsigned char* pixels = new unsigned char[image_width * image_width * 3];

    for (int y = image_height-1; y >= 0; --y) {

        for (int x = 0; x < image_width; ++x) {

            int i = y * image_width + x;
            float u = float(x) / (image_width-1);
            float v = float(y) / (image_height-1);

            Ray r(origin, lower_left_corner + u*horizontal + v*vertical);

            color pixel = ray_color(r);

            pixels[i * 3 + 0] = char(pixel.r * 255.999);
            pixels[i * 3 + 1] = char(pixel.g * 255.999);
            pixels[i * 3 + 2] = char(pixel.b * 255.999);

        }
    }
    
    savePng("raytracer.png", pixels, image_width, image_height, 3);


    return 1;
}