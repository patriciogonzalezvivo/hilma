#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Ray.h"
#include "hilma/types/Mesh.h"
#include "hilma/ops/generate.h"
#include "hilma/ops/transform.h"
#include "hilma/ops/intersection.h"
#include "hilma/io/obj.h"
#include "hilma/io/ply.h"
#include "hilma/io/png.h"


using namespace hilma;
using namespace glm;

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

int main(int argc, char **argv) {

    // Set up conext
    const float aspect_ratio = 16.0f / 9.0f;
    const int image_width = 512;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    point3 origin(0.0f, 0.0f, 0.0f);
    vec3 horizontal(4.0f, 0.0f, 0.0f);
    vec3 vertical(0.0f, 2.25f, 0.0f);
    point3 lower_left_corner = origin - horizontal * 0.5f - vertical * 0.5f - vec3(0.0f,0.0f,1.0f);
    unsigned char* pixels = new unsigned char[image_width * image_width * 3];

    // Scene
    Mesh mesh = icosphere(0.5, 2);
    translate(mesh, 0.0f, 0.0f, -1.0f);
    std::vector<Triangle> triangles = mesh.getTriangles();

    for (int y = image_height-1; y >= 0; --y) {

        for (int x = 0; x < image_width; ++x) {

            int i = y * image_width + x;
            float u = float(x) / (image_width-1);
            float v = float(y) / (image_height-1);

            Ray r(origin, lower_left_corner + u*horizontal + v*vertical);

            color pixel = [&]() {
                float t, u, v;
                for (size_t i = 0; i < triangles.size(); i++) {
                    if (intersectionMT(r, triangles[i], t, u, v)) {
                        return triangles[i].getNormal(u, v) * 0.5f + 0.5f;
                    }
                }

                vec3 unit_direction = normalize(r.getDirection());
                t = 0.5f * (unit_direction.y + 1.0f);
                return (1.0f-t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
            }();

            pixels[i * 3 + 0] = char(pixel.r * 255.999);
            pixels[i * 3 + 1] = char(pixel.g * 255.999);
            pixels[i * 3 + 2] = char(pixel.b * 255.999);

        }
    }
    
    savePng("raytracer.png", pixels, image_width, image_height, 3);


    return 1;
}