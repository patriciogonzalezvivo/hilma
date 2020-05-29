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
#include "hilma/ops/convert_path.h"
#include "hilma/ops/raytrace.h"
#include "hilma/ops/intersection.h"

#include "hilma/io/png.h"
#include "hilma/io/jpg.h"
#include "hilma/io/obj.h"
#include "hilma/io/ply.h"

using namespace hilma;

glm::vec3 ray_color(const Ray& _ray, const std::vector<Hittable>& _hittables, int _depth) {
    if (_depth <= 0)
        return glm::vec3(0.0f);

    HitRecord rec;
    if ( hit(_ray, 0.001, 100.0, _hittables, rec) ) {
        if (rec.line != nullptr)
            return glm::vec3(2.0f);
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
    const int maxDepth = 1;
    Image image = Image(image_width, static_cast<int>(image_width / aspect_ratio), 3);

    // CAMERA
    // glm::vec3 lookfrom(3.5f, 2.5f, 3.0f);
    glm::vec3 lookfrom(3.5f, 0.5f, 3.0f);
    glm::vec3 lookat(0.0f, 0.0f, 0.0f);
    glm::vec3 vup(0.0f, -1.0f, 0.0f);
    float dist_to_focus = glm::length(lookfrom-lookat);
    float dov = 35.0f;
    float aperture = 0.5;

    Camera cam = Camera(lookfrom, lookat, vup, dov, aspect_ratio, aperture, dist_to_focus);

    // SCENE
    int branches = 10;
    bool debug = false;

    std::vector<Hittable> scene;


    Mesh head = loadPly("head.ply");
    center(head);
    scale(head, 0.3f);
    // translateY(head, 0.4f);
    // // scene.push_back( Hittable(head.getTriangles(), branches, debug) );
    std::vector<Line> lines = toLines( head.getTriangles() );
    scene.push_back( Hittable(lines, branches) );

    // Mesh icosphere = hilma::icosphere(0.5f, 2);
    // scale(icosphere, 2.0f);
    // scene.push_back( Hittable(icosphere.getTriangles(), branches, debug) );
    // std::vector<Line> lines = toLines( icosphere.getTriangles() );
    // scene.push_back( Hittable(lines, branches) );

    Mesh cone = hilma::cone(0.5f, 1.f, 36, 1, 1);
    rotateX(cone, PI);
    translateX(cone, -2.0f);
    // scene.push_back( Hittable(cone.getTriangles(), branches, debug) );
    lines = toLines( cone.getTriangles() );
    scene.push_back( Hittable(lines, branches) );

    Mesh cylinder = hilma::cylinder(0.5f, 1.f, 36, 1, 1, true);
    translateX(cylinder, 2.0f);
    // scene.push_back( Hittable(cylinder.getTriangles(), branches, debug) );
    lines = toLines( cylinder.getTriangles() );
    scene.push_back( Hittable(lines, branches) );

    // RAYTRACE
    //
    Timer timer;
    timer.start();
    raytrace_multithread (image, cam, scene, samples, maxDepth, ray_color);
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

    savePng("raytracer_lines.png", image);

    return 0;
}