#pragma once

#include "hilma/math.h"

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"


#include <cstdlib>

inline float randomf() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0f);
}

inline float randomf(float _min, float _max) {
    // Returns a random real in [min,max).
    return _min + (_max-_min)*random();
}

inline glm::vec3 random3(float _min, float _max) {
    return glm::vec3(randomf(_min,_max), randomf(_min,_max), randomf(_min,_max));
}

inline glm::vec3 random_in_unit_sphere() {
    while (true) {
        glm::vec3 p = random3(-1.0f, 1.0f);
        if ( glm::length2(p) > 1.0) continue;
        return p;
    }
}

inline glm::vec3 random_unit_vector() {
    float a = randomf(0.0f, TAU);
    float z = randomf(-1.0f, 1.0f);
    float r = sqrt(1.0f - z*z);
    return glm::vec3(r*cos(a), r*sin(a), z);
}

inline glm::vec3 random_in_hemisphere(const glm::vec3& normal) {
    glm::vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}