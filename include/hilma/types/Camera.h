#pragma once

#include "glm/glm.hpp"

#include "hilma/math.h"
#include "hilma/types/Ray.h"

namespace hilma {

class Camera {
public:
    // Camera():   origin(0.0f, 0.0f, 0.0f),
    //             lower_left_corner(-2.0f, 1.0f, -1.0f),
    //             horizontal(4.0f, 0.0f, 0.0f),
    //             vertical(0.0f, -2.0f, 0.0f) { }

    Camera(
            const glm::vec3& _lookfrom, const glm::vec3& _lookat, const glm::vec3& _vup,
            float _vfov, // vertical field-of-view in degrees
            float _aspect_ratio, float _aperture, float _focus_dist) {
            origin = _lookfrom;
            lens_radius = _aperture * 0.5f;

            float theta = glm::radians(_vfov);
            float half_height = tan(theta/2.0f);
            float half_width = _aspect_ratio * half_height;

            w = glm::normalize(_lookfrom - _lookat);
            u = glm::normalize(glm::cross(_vup, w));
            v = glm::cross(w, u);
            lower_left_corner = origin
                              - half_width * _focus_dist * u
                              - half_height * _focus_dist * v
                              - _focus_dist * w;

            horizontal = 2.0f * half_width * _focus_dist * u;
            vertical = 2.0f * half_height * _focus_dist * v;
        }

    Ray getRay(float s, float t) const {
        glm::vec3 rd = lens_radius * random_in_unit_disk();
        glm::vec3 offset = u * rd.x + v * rd.y;

        return Ray(
            origin + offset,
            lower_left_corner + s * horizontal + t * vertical - origin - offset
        );
    }

private:
    glm::vec3   origin;
    glm::vec3   lower_left_corner;
    glm::vec3   horizontal;
    glm::vec3   vertical;
    glm::vec3   u, v, w;
    float       lens_radius;
};

}
