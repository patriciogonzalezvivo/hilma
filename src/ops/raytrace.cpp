#include "hilma/ops/raytrace.h"

#include <cstdlib>
#include <stdlib.h>
#include <algorithm>

#include "glm/glm.hpp"

#include "hilma/math.h"
#include "hilma/text.h"
#include "hilma/ops/intersection.h"

namespace hilma {

bool triangle_compare(const Triangle& a, const Triangle& b, int axis) {
    return a.getCentroid()[axis] < b.getCentroid()[axis];
}

bool triangle_x_compare (const Triangle& a, const Triangle& b) {
    return triangle_compare(a, b, 0);
}

bool triangle_y_compare (const Triangle& a, const Triangle& b) {
    return triangle_compare(a, b, 1);
}

bool triangle_z_compare (const Triangle& a, const Triangle& b) {
    return triangle_compare(a, b, 2);
}

bool line_compare(const Line& a, const Line& b, int axis) {
    return a.getCentroid()[axis] < b.getCentroid()[axis];
}

bool line_x_compare (const Line& a, const Line& b) {
    return line_compare(a, b, 0);
}

bool line_y_compare (const Line& a, const Line& b) {
    return line_compare(a, b, 1);
}

bool line_z_compare (const Line& a, const Line& b) {
    return line_compare(a, b, 2);
}

Hittable::Hittable( const std::vector<Line>& _lines, int _branches) {
    for (size_t i = 0; i < _lines.size(); i++) {
        expand(_lines[i][0]);
        expand(_lines[i][1]);
    }
    expand(0.001f);

    if (_branches > 0 && _lines.size() > 1) {
        leaf = false;

        // int axis = rand() % 3;
        // auto comparator = (axis == 0) ? line_x_compare
        //                 : (axis == 1) ? line_y_compare
        //                 : line_z_compare;

        float width = getWidth();
        float height = getHeight();
        float depth = getDepth();
        auto comparator = (width > std::max(height, depth) ) ? line_x_compare
                        : (height > std::max(width, depth) ) ? line_y_compare
                        : line_z_compare;

        std::vector<Line> lns = _lines;
        std::sort(lns.begin(), lns.end(), comparator);

        std::size_t const half_size = lns.size() / 2;
        left = std::make_shared<Hittable>(std::vector<Line>(lns.begin(), lns.begin() + half_size), _branches-1);
        right = std::make_shared<Hittable>(std::vector<Line>(lns.begin() + half_size, lns.end()), _branches-1);
    }
    else {
        leaf = true;
        lines = _lines;
    }
}

Hittable::Hittable( const std::vector<Triangle>& _triangles, int _branches, bool _debug) {
    for (size_t i = 0; i < _triangles.size(); i++) {
        expand(_triangles[i][0]);
        expand(_triangles[i][1]);
        expand(_triangles[i][2]);
    }
    expand(0.001f);

    if (_branches > 0 && _triangles.size() > 1) {
        leaf = false;

        // int axis = rand() % 3;
        // auto comparator = (axis == 0) ? triangle_x_compare
        //                 : (axis == 1) ? triangle_y_compare
        //                 : triangle_z_compare;

        float width = getWidth();
        float height = getHeight();
        float depth = getDepth();
        auto comparator = (width > std::max(height, depth) ) ? triangle_x_compare
                        : (height > std::max(width, depth) ) ? triangle_y_compare
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

int Hittable::getTotalLines() {
    if (leaf)
        return lines.size();
    else
        return left->getTotalLines() + right->getTotalLines();
}

int Hittable::getTotalTriangles() {
    if (leaf)
        return triangles.size();
    else
        return left->getTotalTriangles() + right->getTotalTriangles();
}

Mesh Hittable::getMesh() {
    Mesh mesh;
    if (leaf) {
        if (triangles.size() > 0)
            mesh.addTriangles(&triangles[0], triangles.size());
        
        if (lines.size() > 0)
            mesh.addEdges(&lines[0], lines.size());
    }
    else {
        mesh.append( left->getMesh() );
        mesh.append( right->getMesh() );
    }
    return mesh;
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

        if ( raytrace(_ray, _minDistance, closest_so_far, lines, tmp_rec) ) {
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
        if (intersection(ray, _lines[i], ip, 0.01)) {
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
    }

    return hit_anything;
}

void raytrace(  Image& _image, const Camera& _cam, const std::vector<Hittable>& _scene, int _samplesPerPixel, int _maxDepth, 
                std::function<glm::vec3(const Ray&, const std::vector<Hittable>&, int)> _rayColor) {

    const int image_width = _image.getWidth();
    const int image_height = _image.getHeight();
    const float over_samples = 1.0f/_samplesPerPixel;

    const int totalPixels = image_width * image_height;

    std::cout << std::endl;

    for (int y = 0; y < image_height; ++y) {
        for (int x = 0; x < image_width; ++x) {
            int i = y * image_width + x;

            glm::vec3 pixel_color(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < _samplesPerPixel; ++s) {
                float u = (x + randomf()) / (image_width-1);
                float v = (y + randomf()) / (image_height-1);

                Ray ray = _cam.getRay(u, v);
                pixel_color += _rayColor(ray, _scene, _maxDepth);
            }

            pixel_color = pixel_color * over_samples;
            pixel_color = sqrt(pixel_color);
            _image.setColor( _image.getIndex(x, y) , pixel_color);
            
            printProgressBar("RayTracing -", i / float(totalPixels), 100 );
        }
    }

}

}