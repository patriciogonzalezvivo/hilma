#pragma once

#include <vector>
#include <iostream>
#include <memory>

#include "hilma/types/Ray.h"
#include "hilma/types/Triangle.h"

#include "hilma/ops/convert.h"

namespace hilma {

//
//  This examples follow the design of @Peter_shirley's book
//  Ray Tracing in One Weekend https://raytracing.github.io/books/RayTracingInOneWeekend.html
//

struct HitRecord {
    glm::vec3           position;
    glm::vec3           barycentric;
    glm::vec3           normal;
    float               distance    = -1.0f;

    TriangleConstPtr    triangle    = nullptr;
    LineConstPtr        line        = nullptr;

    bool                frontFace   = false;
};

class Hittable : public BoundingBox {
public:
    Hittable( const std::vector<Triangle>& _triangles, int _brances, bool _debug = false);

    virtual bool hit(const Ray& _ray, float _minDistance, float _maxDistance, HitRecord& _rec) const;
    virtual int  getTotalTriangles();
    virtual Mesh getMesh();

private:
    std::vector<Triangle>       triangles;
    std::vector<Line>           lines;

    std::shared_ptr<Hittable>   left;
    std::shared_ptr<Hittable>   right;
    bool                        leaf;
};

bool raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Line>& _lines,         HitRecord& _rec);
bool raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Triangle>& _triangles, HitRecord& _rec);
bool raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Hittable>& _hittables, HitRecord& _rec);

}