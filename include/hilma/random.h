#pragma once

#include <cstdlib>
#include "glm/glm.hpp"

inline float randomf() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline float randomf(float _min, float _max) {
    // Returns a random real in [min,max).
    return _min + (_max-_min)*random();
}

inline glm::vec3 randomVec3(float _min, float _max) {
    return glm::vec3(randomf(_min,_max), randomf(_min,_max), randomf(_min,_max));
}