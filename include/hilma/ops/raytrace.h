#pragma once

#include <vector>

#include "hilma/types/Ray.h"
#include "hilma/types/Triangle.h"

namespace hilma {

//
//  This examples follow the design of @Peter_shirley's book
//  Ray Tracing in One Weekend https://raytracing.github.io/books/RayTracingInOneWeekend.html
//

struct hit_record {
    glm::vec3   position;
    glm::vec3   normal;
    float       distance;
    bool        frontFace;

    void        setNormal(const Ray& _ray, const glm::vec3& outward_normal);
};

bool        raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const Triangle& _triangle, hit_record& _rec);
bool        raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Triangle>& _triangles, hit_record& _rec);
glm::vec3   raytrace(const Ray& _ray, const std::vector<Triangle>& _triangles, int _depth);

}