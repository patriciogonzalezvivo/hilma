#pragma once

#include <vector>

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
    MaterialConstPtr    material;
    glm::vec3           position;
    glm::vec3           normal;
    float               distance;
    bool                frontFace;

    void    setNormal(const Ray& _ray, const glm::vec3& outward_normal);
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

// glm::vec3   raytrace(const Ray& _ray, const std::vector<Triangle>& _triangles, int _depth);
glm::vec3   raytrace(const Ray& _ray, const std::vector<Hittable>& _hittables, int _depth);

}