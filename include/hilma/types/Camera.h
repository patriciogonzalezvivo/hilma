#pragma once

#include "glm/glm.hpp"

#include "hilma/types/Ray.h"

namespace hilma {

class Camera {
public:
    Camera():   origin(0.0f, 0.0f, 0.0f),
                lower_left_corner(-2.0f, 1.0f, -1.0f),
                horizontal(4.0f, 0.0f, 0.0f),
                vertical(0.0f, -2.0f, 0.0f) { }

    Ray         getRay(float u, float v) const {
        return  Ray(origin, lower_left_corner + horizontal*u + vertical*v - origin);
    }

private:
    glm::vec3   origin;
    glm::vec3   lower_left_corner;
    glm::vec3   horizontal;
    glm::vec3   vertical;
};

}
