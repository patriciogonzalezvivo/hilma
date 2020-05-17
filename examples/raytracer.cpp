#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/timer.h"
#include "hilma/text.h"
#include "hilma/random.h"

#include "hilma/types/Ray.h"
#include "hilma/types/Camera.h"

#include "hilma/ops/generate.h"
#include "hilma/ops/transform.h"
#include "hilma/ops/intersection.h"
#include "hilma/io/obj.h"
#include "hilma/io/ply.h"
#include "hilma/io/png.h"

using namespace hilma;
using namespace glm;

//
//  This examples follow the amazing book of @Peter_shirley
//  Ray Tracing in One Weekend 
//  https://raytracing.github.io/books/RayTracingInOneWeekend.html
//

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color
const float infinity = std::numeric_limits<float>::infinity();

struct hit_record {
    point3  p;
    vec3    normal;
    float   t;
    bool front_face;

    inline void set_face_normal(const Ray& _ray, const vec3& outward_normal) {
        front_face = dot(_ray.getDirection(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};

bool hit(const Ray& _ray, float t_min, float t_max, const Triangle& _triangle, hit_record& _rec) { 
    float tU, tV;
    if ( intersectionMT(_ray, _triangle, _rec.t, tU, tV) ) {
        if (_rec.t > t_min && _rec.t < t_max) {
            // _rec.normal = reflect(_ray.getDirection(), _triangle.getNormal());
            // _rec.normal = _triangle.getNormal();
            // _rec.set_face_normal(_ray, _triangle.getNormal(tU, tV));
            _rec.normal = _triangle.getNormal(tU, tV);
            _rec.p = _ray.getAt(_rec.t);
            return true;
        }
    }
    return false;
}

bool hit_list(const Ray& _ray, float t_min, float t_max, const std::vector<Triangle>& _triangles, hit_record& _rec) {

    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (size_t i = 0; i < _triangles.size(); i++) {
        hit_record tmp_rec;
        if (glm::dot(_ray.getDirection(), _triangles[i].getNormal()) <= 0)
        {
            if ( hit(_ray, t_min, closest_so_far, _triangles[i], tmp_rec) ) {
                if (_rec.t < closest_so_far) {
                    hit_anything = true;
                    closest_so_far = tmp_rec.t;
                    _rec = tmp_rec;
                }
            }
        }
    }

    return hit_anything;
}

color ray_color(const Ray& _ray, const std::vector<Triangle>& _triangles, int _depth) {
    if (_depth <= 0)
        return color(0.0f);

    hit_record rec;
    if ( hit_list(_ray, 0.0, infinity, _triangles, rec) ) {
        // return color(rec.t);
        // return 0.5f * rec.p + 0.5f;
        // return 0.5f * rec.normal + 0.5f;
        // point3 target = rec.normal + random3(-1.0, 1.0) * 0.1f;
        // point3 target = rec.normal + random_unit_vector();
        point3 target = random_in_hemisphere(rec.normal);
        return ray_color( Ray(rec.p, target), _triangles, _depth-1) * 0.5f;
    }

    vec3 unit_direction = normalize(_ray.getDirection());
    float t = 0.5*(unit_direction.y + 1.0f);
    return (1.0f-t) * color(1.0f) + t * color(0.5f, 0.7f, 1.0f);
}

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

            color pixel_color(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < samples_per_pixel; ++s) {
                float u = (x + randomf()) / (image_width-1);
                float v = (y + randomf()) / (image_height-1);

                Ray ray = cam.getRay(u, v);
                pixel_color += ray_color(ray, triangles, max_depth);
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