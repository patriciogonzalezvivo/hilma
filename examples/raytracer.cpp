#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"

#include "hilma/math.h"
#include "hilma/timer.h"

#include "hilma/types/Camera.h"
#include "hilma/types/Image.h"

#include "hilma/ops/generate.h"
#include "hilma/ops/transform.h"
#include "hilma/ops/raytrace.h"
#include "hilma/ops/intersection.h"

#include "hilma/io/png.h"
#include "hilma/io/jpg.h"
#include "hilma/io/obj.h"
#include "hilma/io/ply.h"
#include "hilma/io/gltf.h"

using namespace hilma;

glm::vec3 ray_color(const Ray& _ray, const std::vector<Hittable>& _hittables, int _depth) {
    if (_depth <= 0)
        return glm::vec3(0.0f);

    HitRecord rec;
    if ( hit(_ray, 0.001, 1000.0, _hittables, rec) ) {

        if (!rec.frontFace)
            return glm::vec3(0.0f);

        if (rec.line != nullptr)
            return glm::vec3(2.0f);

        glm::vec3 diffuse = glm::vec3(1.0f);
        glm::vec3 emissive = glm::vec3(0.0f);
        glm::vec3 normal = rec.normal;
        float opacity = 1.0f;
        float metallic = 0.0f;
        float roughtness = 1.0f;

        if (rec.triangle != nullptr) {
            normal = rec.triangle->getNormal(rec.barycentric);
            diffuse = rec.triangle->getColor(rec.barycentric);

            if (rec.triangle->material != nullptr) {
                bool haveUV = rec.triangle->haveTexCoords();
                glm::vec2 uv;

                if (haveUV) uv = rec.triangle->getTexCoord(rec.barycentric);
                // uv.x = 1.0f - uv.x;

                if ( rec.triangle->material->haveProperty("emissive") )
                    if ( haveUV ) emissive = rec.triangle->material->getColor("emissive", uv);
                    else emissive = rec.triangle->material->getColor("emissive");
                
                if ( rec.triangle->material->haveProperty("roughness") )
                    roughtness = rec.triangle->material->getValue("roughness", uv);

                if ( rec.triangle->material->haveProperty("metallic") )
                    metallic = rec.triangle->material->getValue("metallic", uv);

                if ( rec.triangle->material->haveProperty("opacity") )
                    opacity = rec.triangle->material->getValue("opacity", uv);
            }
        }

        glm::vec3 dir = glm::normalize(_ray.getDirection());
        glm::vec3 reflected = glm::reflect(dir, normal);
        glm::vec3 target = glm::mix(normal, reflected, metallic);
        target += random_unit_vector() * roughtness;

        Ray scattered(rec.position, target);
        return emissive + diffuse * ray_color( scattered, _hittables, _depth-1 );
    }

    return glm::vec3(0.0f);
    // glm::vec3 unit_direction = normalize(_ray.getDirection() );
    // float t = 0.5f * (unit_direction.y + 1.0f);
    // return glm::mix(glm::vec3(1.0f), glm::vec3(0.5f, 0.7f, 1.0f), t);
}

int main(int argc, char **argv) {

    // IMAGE
    const float aspect_ratio = 16.0f / 9.0f;
    const int image_width = 1024;
    const int samples = 50;
    const int maxDepth = 50;
    Image image = Image(image_width, static_cast<int>(image_width / aspect_ratio), 3);

    // CAMERA
    glm::vec3 lookfrom(3.5f, 0.5f, 3.0f);
    glm::vec3 lookat(0.0f, 0.0f, 0.0f);
    glm::vec3 vup(0.0f, -1.0f, 0.0f);
    float dist_to_focus = glm::length(lookfrom-lookat);
    float dov = 35.0f;
    float aperture = 0.15;
    aperture = 0.0;

    Camera cam = Camera(lookfrom, lookat, vup, dov, aspect_ratio, aperture, dist_to_focus);

    // SCENE
    int branches = 10;

    std::vector<Hittable> scene;
    Mesh scene_mesh;

    Material metal = Material("Metal");
    metal.set("metallic", 1.0f);
    metal.set("roughness", 0.0f);

    Material checker = Material("Checker");
    checker.set("diffuse", "default.png");

    Material light = Material("Light");
    light.set("emissive", glm::vec3(1.0f));

    // Material earth = Material("earth");
    // earth.set("diffuse", "earth.jpg");
    // earth.set("roughness", "earth.png");

    // Mesh cornell = loadObj("CornellBox.obj");
    // center(cornell);
    // translateY(cornell, 0.4f);
    // translateZ(cornell, -2.0f);
    // saveObj("cornell.obj", cornell);
    // scene.push_back( Hittable(cornell, branches) );

    // Image heightmap;
    // loadPng("gale.png", heightmap, 1);
    // Mesh terrain = toTerrain(heightmap, 100.0, 0.05);
    // terrain.setMaterial(checker);
    // center(terrain);
    // rotateX(terrain, -PI/2.0f);
    // scale(terrain, 0.1);
    // translateY(terrain, -1.0f);
    // scene.push_back( Hittable(terrain, branches) );

    Mesh light_area = hilma::plane(4.0f, 4.0f, 1, 1);
    light_area.setMaterial(light);
    rotateX(light_area, PI/2.0f);
    translateY(light_area, 2.0f);
    scene.push_back( Hittable(light_area.getTriangles(), 0) );
    scene_mesh.append(light_area);

    Mesh plane = hilma::plane(10.0f, 10.0f, 1, 1);
    plane.setMaterial(checker);
    translateZ(plane, -0.6f);
    rotateX(plane, -PI/2.0f);
    scene.push_back( Hittable(plane.getTriangles(), 0) );
    scene_mesh.append(plane);

    // Mesh head = loadPly("head.ply");
    // center(head);
    // scale(head, 0.15f);
    // translateY(head, 0.4f);
    // scene.push_back( Hittable(head.getTriangles(), branches) );
    // scene_mesh.append(head);

    // Mesh helmet = Mesh("helmet");
    // loadGltf("helmet.glb", helmet);
    // center(helmet);
    // translateY(helmet, 0.4f);
    // scene.push_back( Hittable(helmet.getTriangles(), branches) );
    // scene_mesh.append(helmet);

    // Mesh Duck = Mesh("Duck");
    // loadGltf("Duck.glb", Duck);
    // center(Duck);
    // scale(Duck, 1.2f);
    // translateY(Duck, 0.2f);
    // scene.push_back( Hittable(Duck.getTriangles(), branches) );
    // scene_mesh.append(Duck);

    Mesh BoomBox = Mesh("BoomBox");
    loadGltf("BoomBox.glb", BoomBox);
    if (!BoomBox.haveTangents())
        BoomBox.computeTangents();
    center(BoomBox);
    scale(BoomBox, 100.0f);
    translateY(BoomBox, 0.4f);
    scene.push_back( Hittable(BoomBox.getTriangles(), branches) );
    scene_mesh.append(BoomBox);

    // Mesh icosphere = hilma::icosphere(0.5f, 2);
    // icosphere.setMaterial(metal);
    // // icosphere.setMaterial(checker);
    // // icosphere.setMaterial(earth);
    // scene.push_back( Hittable(icosphere.getTriangles(), branches) );
    // scene_mesh.append(icosphere);

    // Mesh cone = hilma::cone(0.5f, 1.f, 36, 1, 1);
    // // cone.setMaterial(plastic);
    // // cone.setMaterial(metal);
    // cone.setMaterial(checker);
    // rotateX(cone, PI);
    // translateX(cone, -2.0f);
    // scene.push_back( Hittable(cone.getTriangles(), branches) );
    // scene_mesh.append(cone);

    // Mesh cylinder = hilma::cylinder(0.5f, 1.f, 36, 1, 1, true);
    // // cylinder.setMaterial(metal);
    // cylinder.setMaterial(checker);
    // translateX(cylinder, 2.0f);
    // scene.push_back( Hittable(cylinder.getTriangles(), branches) );
    // scene_mesh.append(cylinder);

    saveObj("raytracer.obj", scene_mesh);
    savePly("raytracer.ply", scene_mesh, false);

    // RAYTRACE
    //
    Timer timer;
    timer.start();
    raytrace_multithread(image, cam, scene, samples, maxDepth, ray_color);
    timer.stop();

    const float time_raycasting = timer.get() / 1000.f;
    std::cout << "                            Rendertime time : " << time_raycasting << " secs" << std::endl;

    int totalTriangles = 0;
    for (size_t i = 0; i < scene.size(); i++)
        totalTriangles += scene[i].getTotalTriangles();   

    std::cout << "                  Total number of triangles : " << totalTriangles << std::endl;
    std::cout << "              Total number of ray-box tests : " << getTotalRayBoundingBoxTests() << std::endl;
    std::cout << "        Total number of ray-triangles tests : " << getTotalRayTriangleTests() << std::endl; 
    std::cout << "Total number of ray-triangles intersections : " << getTotalRayTrianglesIntersections() << std::endl;
    std::cout << "            Total number of ray-lines tests : " << getTotalLineLineTests() << std::endl; 
    std::cout << "    Total number of ray-lines intersections : " << getTotalLineLineIntersections() << std::endl;

    savePng("raytracer.png", image);

    // PRINT NORMAL and ALBEDO FOR DENOISER
    cam = Camera(lookfrom, lookat, vup, dov, aspect_ratio, 0.0f, dist_to_focus);

    hilma::Image normal= Image(image);
    raytrace_multithread(normal, cam, scene, 10, 1, [](const Ray& _ray, const std::vector<Hittable>& _hittables, int _depth) {
        HitRecord rec;
        if ( hit(_ray, 0.001, 1000.0, _hittables, rec) )
            if (rec.triangle != nullptr)
                return rec.triangle->getNormal(rec.barycentric);
        
        return glm::vec3(0.0f);
    } );
    savePng("raytracer_normal.png", normal);

    hilma::Image albedo = Image(image);
    raytrace_multithread(albedo, cam, scene, 10, 1, [](const Ray& _ray, const std::vector<Hittable>& _hittables, int _depth) {
        HitRecord rec;
        if ( hit(_ray, 0.001, 1000.0, _hittables, rec) ) {
            glm::vec3 diffuse = glm::vec3(1.0f);

            if (rec.triangle != nullptr)
                diffuse = rec.triangle->getColor(rec.barycentric);
            
            return diffuse;
        }
        return glm::vec3(0.0f);
    } );
    savePng("raytracer_albedo.png", albedo);

    // Image denoised = denoise(image, normal, albedo, true);
    // savePng("raytracer_denoised.png", denoised);

    return 0;
}