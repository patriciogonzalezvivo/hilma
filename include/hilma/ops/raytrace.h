#pragma once

#include <vector>
#include <iostream>


#include "hilma/types/Ray.h"
#include "hilma/types/Mesh.h"
#include "hilma/types/Triangle.h"
#include "hilma/types/Material.h"

namespace hilma {

//
//  This examples follow the design of @Peter_shirley's book
//  Ray Tracing in One Weekend https://raytracing.github.io/books/RayTracingInOneWeekend.html
//

struct HitRecord {
    MaterialConstPtr    material    = nullptr;
    glm::vec4           color       = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    glm::vec3           position;
    glm::vec3           normal;
    glm::vec2           texcoord;
    float               distance     = -1.0f;
    bool                frontFace    = false;
    bool                haveColor    = false;
    bool                haveTexcoord = false;
};

class Hittable : public BoundingBox {
public:
    Hittable( const Mesh& _mesh ) {
        for (size_t i = 0; i < _mesh.getVerticesTotal(); i++)
            expand(_mesh.getVertices()[i]);
        triangles = _mesh.getTriangles();
    }
    std::vector<Triangle> triangles;
};

bool raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Triangle>& _triangles, HitRecord& _rec);
bool raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Hittable>& _hittables, HitRecord& _rec);

}