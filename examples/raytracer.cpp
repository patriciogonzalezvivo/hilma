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
#include "hilma/types/Image.h"

#include "hilma/ops/convert.h"
#include "hilma/ops/generate.h"
#include "hilma/ops/transform.h"
#include "hilma/ops/raytrace.h"
#include "hilma/ops/intersection.h"

#include "hilma/io/png.h"
#include "hilma/io/jpg.h"
#include "hilma/io/obj.h"
#include "hilma/io/ply.h"

using namespace hilma;

glm::vec3 raytrace(const Ray& _ray, const std::vector<Hittable>& _hittables, int _depth) {
    if (_depth <= 0)
        return glm::vec3(0.0f);

    HitRecord rec;
    if ( raytrace(_ray, 0.001, 1000.0, _hittables, rec) ) {

        if (!rec.frontFace)
            return glm::vec3(0.0f);

        if (rec.line != nullptr)
            return glm::vec3(2.0f);

        glm::vec3 attenuation = glm::vec3(0.5f);
        glm::vec3 emissive = glm::vec3(0.0f);
        float metallic = 0.0f;
        float roughtness = 1.0f;
        glm::vec3 normal = rec.normal;
        glm::vec3 lambert = random_unit_vector();

        if (rec.triangle != nullptr) {
            normal = rec.triangle->getNormal(rec.barycentric);

            if (rec.triangle->material != nullptr) {
                bool haveUV = rec.triangle->haveTexCoords();
                glm::vec2 uv;

                if (haveUV) uv = rec.triangle->getTexCoord(rec.barycentric);
                uv.x = 1.0f - uv.x;

                if ( rec.triangle->material->haveProperty("diffuse") )
                    if ( haveUV ) attenuation = rec.triangle->material->getColor("diffuse", uv);
                    else attenuation = rec.triangle->material->getColor("diffuse");
                
                if ( rec.triangle->material->haveProperty("emissive") )
                    if ( haveUV ) emissive = rec.triangle->material->getColor("emissive", uv);
                    else emissive = rec.triangle->material->getColor("emissive");
                
                if ( rec.triangle->material->haveProperty("roughness") )
                    roughtness = rec.triangle->material->getValue("roughness", uv);

                if ( rec.triangle->material->haveProperty("metallic") )
                    metallic = rec.triangle->material->getValue("metallic", uv);
            }
        }

        glm::vec3 reflected = glm::reflect(glm::normalize(_ray.getDirection()), rec.normal);
        glm::vec3 target = glm::mix(normal, reflected, metallic);
        target += lambert * roughtness;

        Ray scattered(rec.position, target);
        return emissive + attenuation * raytrace( scattered, _hittables, _depth-1 );
    }


    glm::vec3 unit_direction = normalize(_ray.getDirection() );
    float t = 0.5f * (unit_direction.y + 1.0f);
    return glm::mix(glm::vec3(1.0f), glm::vec3(0.5f, 0.7f, 1.0f), t);
}

int main(int argc, char **argv) {

    // Set up conext
    const float aspect_ratio = 16.0f / 9.0f;
    const int image_width = 1024 * 0.5;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const float samples_per_pixel = 10;
    const float over_samples = 1.0f/samples_per_pixel; 
    const int max_depth = 50;
    bool debug = false;

    // Scene
    glm::vec3 lookfrom(3.5f, 0.5f, 1.5f);
    glm::vec3 lookat(0.0f, 0.0f, 0.0f);
    glm::vec3 vup(0.0f, -1.0f, 0.0f);
    float dist_to_focus = glm::length(lookfrom-lookat);
    float aperture = 0.05;
    Camera cam = Camera(lookfrom, lookat, vup,
                        35.0f,
                        aspect_ratio, aperture, dist_to_focus);

    std::vector<Hittable> scene;

    Material metal = Material("metal");
    metal.set("metallic", 1.0f);
    metal.set("roughness", 0.0f);

    // Material plastic = Material("plastic");
    // plastic.set("diffuse", glm::vec3(0.3,0.3,1.0));
    // plastic.set("roughness", 0.2);

    Material checker = Material("Checker");
    checker.set("diffuse", "default.png");

    Material earth = Material("Checker");
    earth.set("diffuse", "earth.jpg");
    earth.set("roughness", "earth.png");

    // Mesh cornell = loadObj("CornellBox.obj");
    // center(cornell);
    // translateY(cornell, 0.4f);
    // translateZ(cornell, -2.0f);
    // scene.push_back( Hittable(cornell) );

    // Mesh head = loadPly("head.ply");
    // center(head);
    // scale(head, 0.15f);
    // scene.push_back( Hittable(head, true) );

    // Image heightmap;
    // loadPng("gale.png", heightmap, 1);
    // Mesh terrain = toTerrain(heightmap, 100.0, 0.05);
    // terrain.setMaterial(checker);
    // center(terrain);
    // rotateX(terrain, -PI/2.0f);
    // scale(terrain, 0.1);
    // translateY(terrain, -1.0f);
    // scene.push_back( Hittable(terrain) );

    Mesh plane = hilma::plane(6.0f, 6.0f, 1, 1);
    plane.setMaterial(checker);
    translateZ(plane, -0.6f);
    rotateX(plane, -PI/2.0f);
    scene.push_back( Hittable(plane) );

    Mesh icosphere = hilma::icosphere(0.5f, 2);
    // icosphere.setMaterial(metal);
    // icosphere.setMaterial(checker);
    icosphere.setMaterial(earth);
    savePly("icosphere.ply", icosphere, true);
    scene.push_back( Hittable(icosphere, debug) );

    // Mesh cone = hilma::cone(0.5f, 1.f, 36, 1, 1);
    // // cone.setMaterial(plastic);
    // cone.setMaterial(metal);
    // rotateX(cone, PI);
    // translateX(cone, -2.0f);
    // scene.push_back( Hittable(cone, debug) );

    // Mesh cylinder = hilma::cylinder(0.5f, 1.f, 36, 1, 1, true);
    // cylinder.setMaterial(metal);
    // translateX(cylinder, 2.0f);
    // scene.push_back( Hittable(cylinder, debug) );

    // RAYTRACER
    //
    Timer timer;
    timer.start();
    std::cout << std::endl;
    
    const int totalPixels = image_width * image_height;
    const float totalRays = image_width * image_height * samples_per_pixel;
    Image image = Image(image_width, image_height, 3);

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
            image.setColor(x, y, pixel_color);
            
            printProgressBar("RayTracing -", i / float(totalPixels), 100 );

        }
    }

    timer.stop();

    const float time_raycasting = timer.get() / 1000.f;
    std::cout << "                            Rendertime time : " << time_raycasting << " secs" << std::endl;

    int totalTriangles = 0;
    for (size_t i = 0; i < scene.size(); i++)
        totalTriangles += scene[i].triangles.size();    
    std::cout << "                  Total number of triangles : " << totalTriangles << std::endl;

    std::cout << "              Total number of ray-box tests : " << getTotalRayBoundingBoxTests() << std::endl;
    std::cout << "        Total number of ray-triangles tests : " << getTotalRayTriangleTests() << std::endl; 
    std::cout << "Total number of ray-triangles intersections : " << getTotalRayTrianglesIntersections() << std::endl;
    std::cout << "            Total number of ray-lines tests : " << getTotalLineLineTests() << std::endl; 
    std::cout << "    Total number of ray-lines intersections : " << getTotalLineLineIntersections() << std::endl;

    savePng("raytracer.png", image);

    return 0;
}