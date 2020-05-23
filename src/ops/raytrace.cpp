#include "hilma/ops/raytrace.h"

#include "glm/glm.hpp"

#include <cstdlib>

#include "hilma/math.h"
#include "hilma/ops/intersection.h"

namespace hilma {

// Ray / Line

bool raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Line>& _lines, HitRecord& _rec) {
    Line ray = Line(_ray.getOrigin(), _ray.getAt(std::min(100.0f, _maxDistance)));

    HitRecord tmp_rec;
    bool hit_anything = false;
    float closest_so_far = _maxDistance;

    glm::vec3 ip;
    for (size_t i = 0; i < _lines.size(); i++) {
        if (intersection(ray, _lines[i], ip, 0.1)) {
            tmp_rec.distance = glm::length(_ray.getOrigin() - ip);
            if (tmp_rec.distance < closest_so_far) {
                closest_so_far = tmp_rec.distance;
                tmp_rec.line = std::make_shared<Line>(_lines[i]);
                tmp_rec.frontFace = true;
                _rec = tmp_rec;
                hit_anything = true;
            }
        }
    }

    return hit_anything;
}

// RAY / TRIANGLE 

bool raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const Triangle& _triangle, HitRecord& _rec) { 
    float distance, u, v;
    if ( intersection(_ray, _triangle, distance, u, v) ) {
        if (distance > _minDistance && distance < _maxDistance ) {

            _rec.distance = distance;
            _rec.position = _ray.getAt(distance);
            _rec.barycentric = glm::vec3((1.0f - u - v), u, v);
            _rec.normal = _triangle.getNormal();

            _rec.frontFace = glm::dot(_ray.getDirection(), _rec.normal) < 0;
            _rec.normal = _rec.frontFace ? _rec.normal :-_rec.normal;

            _rec.triangle = std::make_shared<Triangle>(_triangle);

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


// RAY HITTABLE

bool raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Hittable>& _hittables, HitRecord& _rec) {

    bool hit_anything = false;
    float closest_so_far = _maxDistance;

    HitRecord tmp_rec;
    float tmin, tmax;
    // foreach hittable
    for (size_t i = 0; i < _hittables.size(); i++) {

        // if ( intersection(_ray, static_cast<Hittable>(_hittables[i]), tmin, tmax) ) 
        // {

        //     for (size_t j = 0; j < _hittables[i].triangles.size(); j++) {
        //         if ( raytrace(_ray, _minDistance, closest_so_far, _hittables[i].triangles[j], tmp_rec) ) {
        //             if ( tmp_rec.distance < closest_so_far ) {
        //                 hit_anything = true;
        //                 closest_so_far = tmp_rec.distance;
        //                 _rec = tmp_rec;
        //             }   
        //         }
        //     }
        // }

        // if ( intersection(_ray, static_cast<Hittable>(_hittables[i]), tmin, tmax) ) 
        if ( raytrace(_ray, _minDistance, closest_so_far, _hittables[i].triangles, tmp_rec) ) {
            if ( tmp_rec.distance < closest_so_far ) {
                hit_anything = true;
                closest_so_far = tmp_rec.distance;
                _rec = tmp_rec;
            }
        }

        if (raytrace(_ray, _minDistance, _maxDistance, _hittables[i].lines, tmp_rec) ) {
            if (tmp_rec.distance < closest_so_far) {
                hit_anything = true;
                closest_so_far = tmp_rec.distance;
                _rec = tmp_rec;
            }
        }
    }

    return hit_anything;
}

}