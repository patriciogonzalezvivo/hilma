#pragma once

#include <vector>
#include <iostream>


#include "hilma/types/Ray.h"
#include "hilma/types/Mesh.h"
#include "hilma/types/Triangle.h"

namespace hilma {

//
//  This examples follow the design of @Peter_shirley's book
//  Ray Tracing in One Weekend https://raytracing.github.io/books/RayTracingInOneWeekend.html
//

struct HitRecord {
    glm::vec3           position;
    glm::vec3           normal;
    float               distance    = -1.0f;
    TriangleConstPtr    triangle    = nullptr;
    bool                frontFace   = false;
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