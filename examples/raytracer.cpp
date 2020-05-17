#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/timer.h"
#include "hilma/text.h"

#include "hilma/types/Camera.h"

#include "hilma/ops/generate.h"
#include "hilma/ops/transform.h"
#include "hilma/ops/raytrace.h"

#include "hilma/io/ply.h"
#include "hilma/io/png.h"

using namespace hilma;
using namespace glm;

int main(int argc, char **argv) {

    // Set up conext
    const float aspect_ratio = 16.0f / 9.0f;
    const int image_width = 1024*0.5;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const float samples_per_pixel = 4;
    const float over_samples = 1.0f/samples_per_pixel; 
    const int max_depth = 50;

    // Scene
    Camera cam;
    Mesh scene;

    // Mesh head = loadPly("head.ply");
    // center(head);
    // scale(head, 0.1);
    // scene.append(head);    
    
    Mesh icosphere = hilma::icosphere(0.5f, 2);
    scene.append(icosphere);

    // Mesh cone = hilma::cone(0.5f, 1.f, 36, 1, 1);
    // rotateX(cone, PI);
    // translateX(cone, -2.0f);
    // scene.append(cone);

    // Mesh cylinder = hilma::cylinder(0.5f, 1.f, 36, 1, 1, true);
    // translateX(cylinder, 2.0f);
    // scene.append(cylinder);
    
    Mesh plane = hilma::plane(6.0f, 6.0f, 1, 1);
    translateZ(plane, -0.6f);
    rotateX(plane, -PI/2.);
    scene.append(plane);

    savePly("raytracer.ply", scene, false);

    translate(scene, 0.0f, 0.0f, -1.0f);
    std::vector<Triangle> triangles = scene.getTriangles();

    Timer timer;
    timer.start();
    std::cout << std::endl;

    const std::string deleteLine = "\e[2K\r\e[1A";
    const int totalPixels = image_width * image_height;
    const float totalRays = image_width * image_height * samples_per_pixel;
    unsigned char* pixels = new unsigned char[totalPixels * 3];
    for (int y = 0; y < image_height; ++y) {
        for (int x = 0; x < image_width; ++x) {

            int i = y * image_width + x;

            glm::vec3 pixel_color(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < samples_per_pixel; ++s) {
                float u = (x + randomf()) / (image_width-1);
                float v = (y + randomf()) / (image_height-1);

                Ray ray = cam.getRay(u, v);
                pixel_color += raytrace(ray, triangles, max_depth);
            }

            pixel_color = pixel_color * over_samples;
            pixel_color = sqrt(pixel_color);
            pixels[i * 3 + 0] = static_cast<char>(256 * clamp(pixel_color.r, 0.0, 0.999));
            pixels[i * 3 + 1] = static_cast<char>(256 * clamp(pixel_color.g, 0.0, 0.999));
            pixels[i * 3 + 2] = static_cast<char>(256 * clamp(pixel_color.b, 0.0, 0.999));

            std::cout << deleteLine;
            std::cout << " RayTracing - [ ";
            int pct = (i / float(totalPixels)) * 100;
            for (int i = 0; i < 50; i++) {
                if (i < pct/2) std::cout << "#";
                else std::cout << ".";
            }
            std::cout << " ]" << std::endl;

        }
    }

    timer.stop();
    const float time_raycasting = timer.get() / 1000.f;
    std::cout << "Casting all rays: " << time_raycasting << " secs ( " << toString(time_raycasting / totalRays, 10) << "s per ray) "<< std::endl;

    savePng("raytracer.png", pixels, image_width, image_height, 3);

    return 0;
}