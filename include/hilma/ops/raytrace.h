#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <functional>

#include "hilma/types/ray.h"
#include "hilma/types/mesh.h"
#include "hilma/types/image.h"
#include "hilma/types/camera.h"

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
    // Hittable( const Mesh& _mesh, int _branches);
    Hittable( const std::vector<Line>& _lines, int _branches);
    Hittable( const std::vector<Triangle>& _triangles, int _branches);

    virtual bool hit(const Ray& _ray, float _minDistance, float _maxDistance, HitRecord& _rec) const;
    // virtual glm::vec3 closest(const glm::vec3& _point);

    virtual int  getTotalTriangles();
    virtual int  getTotalLines();
    virtual Mesh getMesh();

private:
    std::vector<Triangle>       triangles;
    std::vector<Line>           lines;

    std::shared_ptr<Hittable>   left;
    std::shared_ptr<Hittable>   right;
    bool                        leaf;
};

bool hit(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Line>& _lines,         HitRecord& _rec);
bool hit(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Triangle>& _triangles, HitRecord& _rec);
bool hit(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Hittable>& _hittables, HitRecord& _rec);

glm::vec3 default_rayColor(const Ray& _ray, const std::vector<Hittable>& _hittables, int _depth);
glm::vec3 albedo_rayColor(const Ray& _ray, const std::vector<Hittable>& _hittables, int _depth);
glm::vec3 normal_rayColor(const Ray& _ray, const std::vector<Hittable>& _hittables, int _depth);

void raytrace(Image& _image, const Camera& _cam, const std::vector<Hittable>& _scene, int _samples, int _maxDepth, std::function<glm::vec3(const Ray&, const std::vector<Hittable>&, int)> _rayColor = default_rayColor);
void raytrace_multithread(Image& _image, const Camera& _cam, const std::vector<Hittable>& _scene, int _samples, int _maxDepth, std::function<glm::vec3(const Ray&, const std::vector<Hittable>&, int)> _rayColor = default_rayColor);

}