#include "hilma/ops/raytrace.h"

#include "glm/glm.hpp"

#include <cstdlib>

#include "hilma/math.h"
#include "hilma/ops/intersection.h"

namespace hilma {

// RAY / TRIANGLE 

bool raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const Triangle& _triangle, HitRecord& _rec) { 
    float distance, u, v;
    if ( intersectionMT(_ray, _triangle, distance, u, v) ) {
        if (distance > _minDistance && distance < _maxDistance ) {

            _rec.distance = distance;
            _rec.position = _ray.getAt( distance );
            _rec.normal = _triangle.getNormal(u, v);
            _rec.frontFace = glm::dot(_ray.getDirection(), _rec.normal) < 0;
            _rec.normal = _rec.frontFace ? _rec.normal :-_rec.normal;

            // if (_triangle.material!= nullptr)
            _rec.material = _triangle.material;
            
            if (_triangle.haveColors()) {
                _rec.color = _triangle.getColor(u, v);
                _rec.haveColor = true;
            }
            
            if (_triangle.haveTexCoords()) {
                _rec.texcoord = _triangle.getTexCoord(u, v);
                _rec.haveTexcoord = true;
            }

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

}