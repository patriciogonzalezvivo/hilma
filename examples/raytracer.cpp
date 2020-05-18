#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"

#include "hilma/math.h"
#include "hilma/text.h"
#include "hilma/timer.h"

#include "hilma/types/Camera.h"

#include "hilma/ops/generate.h"
#include "hilma/ops/transform.h"
#include "hilma/ops/raytrace.h"

#include "hilma/io/obj.h"
#include "hilma/io/ply.h"
#include "hilma/io/png.h"

using namespace hilma;

const float infinity = std::numeric_limits<float>::infinity();

glm::vec3 random3(float _min, float _max) {
    return glm::vec3(randomf(_min,_max), randomf(_min,_max), randomf(_min,_max));
}

glm::vec3 random_in_unit_sphere() {
    while (true) {
        glm::vec3 p = random3(-1.0f, 1.0f);
        if ( glm::length2(p) > 1.0) continue;
        return p;
    }
}

glm::vec3 random_unit_vector() {
    float a = randomf(0.0f, TAU);
    float z = randomf(-1.0f, 1.0f);
    float r = sqrt(1.0f - z*z);
    return glm::vec3(r*cos(a), r*sin(a), z);
}

glm::vec3 random_in_hemisphere(const glm::vec3& normal) {
    glm::vec3 in_unit_sphere = random_in_unit_sphere() ;
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

glm::vec3 raytrace(const Ray& _ray, const std::vector<Hittable>& _hittables, int _depth) {
    if (_depth <= 0)
        return glm::vec3(0.0f);

    HitRecord rec;
    if ( raytrace(_ray, 0.001, infinity, _hittables, rec) ) {

        if (!rec.frontFace)
            return glm::vec3(0.0f);

        glm::vec3 attenuation = glm::vec3(0.5f);
        glm::vec3 emission = glm::vec3(0.0f);
        glm::vec3 target = rec.normal;
        glm::vec3 lambert = random_unit_vector();

        if (rec.material != nullptr) {
            attenuation = rec.material->diffuse;
            emission = rec.material->emissive * 10.0f;
            glm::vec3 reflected = glm::reflect(glm::normalize(_ray.getDirection()), rec.normal);
            target = glm::mix(target, reflected, rec.material->metallic);
            target += lambert * (0.5f + rec.material->roughness);
        }
        else
            target += lambert;

        Ray scattered(rec.position, target);
        return emission + attenuation * raytrace( scattered, _hittables, _depth-1 );
    }

    glm::vec3 unit_direction = normalize(_ray.getDirection() );
    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

int main(int argc, char **argv) {

    // Set up conext
    const float aspect_ratio = 16.0f / 9.0f;
    const int image_width = 1024 * 0.5;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const float samples_per_pixel = 50;
    const float over_samples = 1.0f/samples_per_pixel; 
    const int max_depth = 50;

    // Scene
    Camera cam;

    std::vector<Hittable> scene;

    Mesh cornell = loadObj("CornellBox.obj");
    center(cornell);
    translateY(cornell, 0.4f);
    translateZ(cornell, -2.0f);
    scene.push_back( Hittable(cornell) );

    Material metal = Material("metal");
    metal.diffuse = glm::vec3(0.5);
    metal.metallic = 1.0f;
    metal.roughness = 0.0f;
    metal.diffuse = glm::vec3(1.0f);

    Material plastic = Material("plastic");
    plastic.diffuse = glm::vec3(0.3,0.3,1.0);
    plastic.roughness = 0.2;

    Mesh plane = hilma::plane(6.0f, 6.0f, 1, 1);
    translateZ(plane, -0.6f);
    rotateX(plane, -PI/2.);
    translateZ(plane, -1.0f);
    scene.push_back( Hittable(plane) );

    // Mesh icosphere = hilma::icosphere(0.5f, 2);
    // translateZ(icosphere, -1.0f);
    // scene.push_back( Hittable(icosphere) );

    Mesh cone = hilma::cone(0.5f, 1.f, 36, 1, 1);
    cone.setMaterial(plastic);
    rotateX(cone, PI);
    translateX(cone, -2.0f);
    translateZ(cone, -1.0f);
    scene.push_back( Hittable(cone) );

    Mesh cylinder = hilma::cylinder(0.5f, 1.f, 36, 1, 1, true);
    cylinder.setMaterial(metal);
    translateX(cylinder, 2.0f);
    translateZ(cylinder, -1.0f);
    scene.push_back( Hittable(cylinder) );

    Timer timer;
    timer.start();
    std::cout << std::endl;
    
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
                pixel_color += raytrace(ray, scene, max_depth);
            }

            pixel_color = pixel_color * over_samples;
            pixel_color = sqrt(pixel_color);
            pixels[i * 3 + 0] = static_cast<char>(256 * clamp(pixel_color.r, 0.0, 0.999));
            pixels[i * 3 + 1] = static_cast<char>(256 * clamp(pixel_color.g, 0.0, 0.999));
            pixels[i * 3 + 2] = static_cast<char>(256 * clamp(pixel_color.b, 0.0, 0.999));

            printProgressBar("RayTracing -", i / float(totalPixels), 100 );

        }
    }

    timer.stop();
    const float time_raycasting = timer.get() / 1000.f;
    std::cout << "Casting all rays: " << time_raycasting << " secs ( " << toString(time_raycasting / totalRays, 10) << "s per ray) "<< std::endl;

    savePng("raytracer.png", pixels, image_width, image_height, 3);

    return 0;
}