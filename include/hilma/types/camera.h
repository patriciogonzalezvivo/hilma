#pragma once

#include "glm/glm.hpp"
#include "hilma/types/ray.h"

namespace hilma {

class Camera {
public:
    // Camera():   origin(0.0f, 0.0f, 0.0f),
    //             lower_left_corner(-2.0f, 1.0f, -1.0f),
    //             horizontal(4.0f, 0.0f, 0.0f),
    //             vertical(0.0f, -2.0f, 0.0f) { }

    Camera( const glm::vec3& _lookfrom, const glm::vec3& _lookat, const glm::vec3& _vup,
            float _vfov, // vertical field-of-view in degrees
            float _aspect_ratio, float _aperture, float _focus_dist);

    Ray getRay(float s, float t) const;

private:
    glm::vec3   origin;
    glm::vec3   lower_left_corner;
    glm::vec3   horizontal;
    glm::vec3   vertical;
    glm::vec3   u, v, w;
    float       lens_radius;
};

}
