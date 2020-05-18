#include "hilma/ops/raytrace.h"

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"

#include <cstdlib>
#include <iostream>

#include "hilma/math.h"
#include "hilma/ops/intersection.h"

namespace hilma {

// HELPING FUNCTIONS

const float infinity = std::numeric_limits<float>::infinity();

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

// HIT RECORD

void HitRecord::setNormal(const Ray& _ray, const glm::vec3& outward_normal) {
    frontFace = glm::dot(_ray.getDirection(), outward_normal) < 0;
    normal = frontFace ? outward_normal :-outward_normal;
}

// RAY / TRIANGLE 

bool raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const Triangle& _triangle, HitRecord& _rec) { 
    float distance, u, v;
    if ( intersectionMT(_ray, _triangle, distance, u, v) ) {
        if (distance > _minDistance && distance < _maxDistance ) {

            _rec.distance = distance;
            _rec.position = _ray.getAt( distance );
            _rec.setNormal(_ray, _triangle.getNormal(u, v));
            if (_triangle.material != nullptr)
                _rec.material = _triangle.material;
            
            return true;
        }
    }
    return false;
}

bool raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Triangle>& _triangles, HitRecord& _rec) {

    bool hit_anything = false;
    float closest_so_far = _maxDistance;

    HitRecord tmp_rec;
    for (size_t i = 0; i < _triangles.size(); i++) {
        // if (glm::dot(_ray.getDirection(), _triangles[i].getNormal()) < 0)
        {
            if ( raytrace(_ray, _minDistance, closest_so_far, _triangles[i], tmp_rec) ) {
                if ( tmp_rec.distance < closest_so_far ) { //&& tmp_rec.frontFace ) {
                    hit_anything = true;
                    closest_so_far = tmp_rec.distance;
                    _rec = tmp_rec;
                }
            }
        }
    }

    return hit_anything;
}


// RAY HITTABLE

bool raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Hittable>& _hittables, HitRecord& _rec) {

    bool hit_anything = false;
    float closest_so_far = _maxDistance;

    float tmin, tmax;
    HitRecord tmp_rec;
    // foreach hittable
    for (size_t i = 0; i < _hittables.size(); i++) {

        // //but not those whos ray origine are insde of it or doesn't intersect their bounding boxes
        // if (_hittables[i].contains(_ray.getOrigin()) ||
        //     intersection(_ray, static_cast<Hittable>(_hittables[i]), tmin, tmax) ) 
        {
            if ( raytrace(_ray, _minDistance, closest_so_far, _hittables[i].triangles, tmp_rec) ) {
                if ( tmp_rec.distance < closest_so_far ) {
                    hit_anything = true;
                    closest_so_far = tmp_rec.distance;
                    _rec = tmp_rec;
                }
            }
        }

    }

    return hit_anything;
}

glm::vec3 raytrace(const Ray& _ray, const std::vector<Hittable>& _hittables, int _depth) {
    if (_depth <= 0)
        return glm::vec3(0.0f);

    HitRecord rec;
    if ( raytrace(_ray, 0.001, infinity, _hittables, rec) ) {

        if (!rec.frontFace)
            return glm::vec3(0.0f);

        glm::vec3 attenuation = glm::vec3(0.5f);
        glm::vec3 emission = glm::vec3(0.0f);
        glm::vec3 target = rec.normal;
        glm::vec3 roughness = random_unit_vector();

        if (rec.material != nullptr) {
            attenuation = rec.material->diffuse;
            emission = rec.material->emissive;
            glm::vec3 reflected = glm::reflect(glm::normalize(_ray.getDirection()), target);
            target = glm::mix(target, reflected, rec.material->metallic);
            target += roughness * (0.5f + rec.material->roughness * 0.5f);
        }
        else
            target += roughness;

        Ray scattered(rec.position, target);

        return emission + attenuation * raytrace( scattered, _hittables, _depth-1);
    }

    glm::vec3 unit_direction = normalize(_ray.getDirection() );
    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

// glm::vec3 raytrace(const Ray& _ray, const std::vector<Triangle>& _triangles, int _depth) {
//     if (_depth <= 0)
//         return glm::vec3(0.0f);

//     HitRecord rec;
//     if ( raytrace(_ray, 0.01, infinity, _triangles, rec) ) {
//         // return color(rec.t);
//         // return 0.5f * rec.p + 0.5f;
//         // return 0.5f * rec.normal + 0.5f;
//         glm::vec3 target = rec.normal + random_unit_vector();
//         // glm::vec3 target = random_in_hemisphere(rec.normal);
//         return raytrace( Ray(rec.position, target), _triangles, _depth-1) * 0.5f;
//     }

//     glm::vec3 unit_direction = normalize(_ray.getDirection() );
//     float t = 0.5f * (unit_direction.y + 1.0f);
//     return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
// }

}