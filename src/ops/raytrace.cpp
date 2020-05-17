#include "hilma/ops/raytrace.h"

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"

#include <cstdlib>

#include "hilma/math.h"
#include "hilma/ops/intersection.h"

namespace hilma {

const float infinity = std::numeric_limits<float>::infinity();

void hit_record::setNormal(const Ray& _ray, const glm::vec3& outward_normal) {
    frontFace = glm::dot(_ray.getDirection(), outward_normal) < 0;
    normal = frontFace ? outward_normal :-outward_normal;
}

bool raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const Triangle& _triangle, hit_record& _rec) { 
    float distance, u, v;
    if ( intersectionMT(_ray, _triangle, distance, u, v) ) {
        if (distance > _minDistance && distance < _maxDistance ) {
            _rec.distance = distance;
            _rec.position = _ray.getAt( distance );
            _rec.setNormal(_ray, _triangle.getNormal());
            return true;
        }
    }
    return false;
}

bool raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Triangle>& _triangles, hit_record& _rec) {

    bool hit_anything = false;
    auto closest_so_far = _maxDistance;

    for (size_t i = 0; i < _triangles.size(); i++) {
        // if (glm::dot(_ray.getDirection(), _triangles[i].getNormal()) < 0)
        {
            hit_record tmp_rec;
            if ( raytrace(_ray, _minDistance, closest_so_far, _triangles[i], tmp_rec) ) {
                if ( tmp_rec.distance < closest_so_far  ) {
                    hit_anything = true;
                    closest_so_far = tmp_rec.distance;
                    _rec = tmp_rec;
                }
            }
        }
    }

    return hit_anything;
}

glm::vec3 random3(float _min, float _max) {
    return glm::vec3(randomf(_min,_max), randomf(_min,_max), randomf(_min,_max));
}

glm::vec3 random_in_unit_sphere() {
    while (true) {
        glm::vec3 p = random3(-1.0f, 1.0f);
        if ( glm::length2(p) > 1.0) continue;
        return p;
    }
}

glm::vec3 random_unit_vector() {
    float a = randomf(0.0f, TAU);
    float z = randomf(-1.0f, 1.0f);
    float r = sqrt(1.0f - z*z);
    return glm::vec3(r*cos(a), r*sin(a), z);
}

glm::vec3 random_in_hemisphere(const glm::vec3& normal) {
    glm::vec3 in_unit_sphere = random_in_unit_sphere() ;
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

glm::vec3 raytrace(const Ray& _ray, const std::vector<Triangle>& _triangles, int _depth) {
    if (_depth <= 0)
        return glm::vec3(0.0f);

    hit_record rec;
    if ( raytrace(_ray, 0.0, infinity, _triangles, rec) ) {
        // return color(rec.t);
        // return 0.5f * rec.p + 0.5f;
        // return 0.5f * rec.normal + 0.5f;
        // glm::vec3 target = rec.p + rec.normal + random3(-1.0, 1.0) * 0.1f;
        glm::vec3 target = rec.position + rec.normal + random_unit_vector();
        // glm::vec3 target = random_in_hemisphere(-rec.normal);
        return raytrace( Ray(rec.position, target), _triangles, _depth-1) * 0.5f;
    }

    glm::vec3 unit_direction = normalize(_ray.getDirection() );
    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

}