#include "hilma/ops/raytrace.h"

#include <cstdlib>
#include <stdlib.h>
#include <algorithm>

#include "glm/glm.hpp"

#include "hilma/math.h"
#include "hilma/ops/intersection.h"

namespace hilma {

bool triangle_compare(Triangle a, Triangle b, int axis) {
    return a.getCentroid()[axis] < b.getCentroid()[axis];
}

bool triangle_x_compare (Triangle a, Triangle b) {
    return triangle_compare(a, b, 0);
}

bool triangle_y_compare (Triangle a, Triangle b) {
    return triangle_compare(a, b, 1);
}

bool triangle_z_compare (Triangle a, Triangle b) {
    return triangle_compare(a, b, 2);
}

Hittable:: Hittable( const std::vector<Triangle>& _triangles, int _branches, bool _debug) {
    for (size_t i = 0; i < _triangles.size(); i++) {
        expand(_triangles[i][0]);
        expand(_triangles[i][1]);
        expand(_triangles[i][2]);
    }
    expand(0.001f);

    if (_branches > 0 && _triangles.size() > 1) {
        leaf = false;
        int axis = rand() % 3;
        auto comparator = (axis == 0) ? triangle_x_compare
                        : (axis == 1) ? triangle_y_compare
                        : triangle_z_compare;

        std::vector<Triangle> tris = _triangles;
        std::sort(tris.begin(), tris.end(), comparator);

        std::size_t const half_size = tris.size() / 2;
        left = std::make_shared<Hittable>(std::vector<Triangle>(tris.begin(), tris.begin() + half_size), _branches-1, _debug);
        right = std::make_shared<Hittable>(std::vector<Triangle>(tris.begin() + half_size, tris.end()), _branches-1, _debug);
    }
    else {
        leaf = true;
        triangles = _triangles;

        if (_debug)
            lines = toLines(*this);
    }

}

int Hittable::getTotalTriangles() {
    if (leaf)
        return triangles.size();
    else
        return left->getTotalTriangles() + right->getTotalTriangles();
}

bool Hittable::hit(const Ray& _ray, float _minDistance, float _maxDistance, HitRecord& _rec) const {
    if ( !intersection(_ray, (BoundingBox)*this, _minDistance, _maxDistance) )
        return false;

    bool hit_anything = false;

    if (!leaf) {
        bool hit_left = left->hit(_ray, _minDistance, _maxDistance, _rec);
        bool hit_right = right->hit(_ray, _minDistance, hit_left ? _rec.distance : _maxDistance, _rec);

        return hit_left || hit_right;
    }
    else {
        HitRecord tmp_rec;
        float closest_so_far = _maxDistance;
        if ( raytrace(_ray, _minDistance, closest_so_far, triangles, tmp_rec) ) {
            if ( tmp_rec.distance < closest_so_far ) {
                hit_anything = true;
                closest_so_far = tmp_rec.distance;
                _rec = tmp_rec;
            }
        }

        if (raytrace(_ray, _minDistance, closest_so_far, lines, tmp_rec) ) {
            if (tmp_rec.distance < closest_so_far) {
                hit_anything = true;
                closest_so_far = tmp_rec.distance;
                _rec = tmp_rec;
            }
        }
    }

    return hit_anything;
}

// Ray / Line

bool raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Line>& _lines, HitRecord& _rec) {
    Line ray = Line(_ray.getOrigin(), _ray.getAt(std::min(100.0f, _maxDistance)));

    HitRecord tmp_rec;
    bool hit_anything = false;
    float closest_so_far = _maxDistance;

    glm::vec3 ip;
    for (size_t i = 0; i < _lines.size(); i++) {
        if (intersection(ray, _lines[i], ip, 0.05)) {
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
bool raytrace(const Ray& _ray, float _minDistance, float _maxDistance, const std::vector<Triangle>& _triangles, HitRecord& _rec) {

    bool hit_anything = false;
    float closest_so_far = _maxDistance;

    HitRecord tmp_rec;
    for (size_t i = 0; i < _triangles.size(); i++) {
        float distance, u, v;
        if ( intersection(_ray, _triangles[i], distance, u, v) ) {
            if (distance > _minDistance && distance < closest_so_far ) {
                hit_anything = true;
                closest_so_far = distance;

                _rec.distance = distance;
                _rec.position = _ray.getAt(distance);
                _rec.barycentric = glm::vec3((1.0f - u - v), u, v);
                _rec.normal = _triangles[i].getNormal();

                _rec.frontFace = glm::dot(_ray.getDirection(), _rec.normal) < 0;
                _rec.normal = _rec.frontFace ? _rec.normal :-_rec.normal;

                _rec.triangle = std::make_shared<Triangle>(_triangles[i]);
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
    // foreach hittable
    for (size_t i = 0; i < _hittables.size(); i++) {
        if ( _hittables[i].hit( _ray, _minDistance, _maxDistance, tmp_rec) ) {
            if (tmp_rec.distance < closest_so_far) {
                hit_anything = true;
                closest_so_far = tmp_rec.distance;
                _rec = tmp_rec;
            }
        }

        // // if ( intersection(_ray, static_cast<Hittable>(_hittables[i]), tmin, tmax) ) 
        // if ( raytrace(_ray, _minDistance, closest_so_far, _hittables[i].triangles, tmp_rec) ) {
        //     if ( tmp_rec.distance < closest_so_far ) {
        //         hit_anything = true;
        //         closest_so_far = tmp_rec.distance;
        //         _rec = tmp_rec;
        //     }
        // }

        // if (raytrace(_ray, _minDistance, _maxDistance, _hittables[i].lines, tmp_rec) ) {
        //     if (tmp_rec.distance < closest_so_far) {
        //         hit_anything = true;
        //         closest_so_far = tmp_rec.distance;
        //         _rec = tmp_rec;
        //     }
        // }
    }

    return hit_anything;
}

}