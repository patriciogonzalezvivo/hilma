#pragma once

#include <math.h>

#include "glm/glm.hpp"

namespace hilma {

inline glm::vec2 toPolar(const glm::vec2& _cartA, const glm::vec2& _cartB) {
    glm::vec2 delta = _cartB - _cartA;
    return glm::vec2(atan2(delta.y,delta.x), glm::length(glm::vec2(delta.x,delta.y)));
}

inline glm::vec3 toPolar(const glm::vec3& _cartA, const glm::vec3& _cartB) {
    glm::vec3 delta = _cartB - _cartA;
    return glm::vec3(atan2(delta.y,delta.x), glm::length(glm::vec2(delta.x,delta.y)), _cartA.z + delta.z * 0.5f);
}

inline glm::vec2 toCartesian(const glm::vec2& _polar) {
    return glm::vec2(_polar.y*cos(_polar.x), _polar.y*sin(_polar.x));
}

inline glm::vec3 toCartesian(const glm::vec3& _polar) {
    return glm::vec3(_polar.y*cos(_polar.x), _polar.y*sin(_polar.x), _polar.z);
}

}