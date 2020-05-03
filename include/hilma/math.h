#pragma once

#include <math.h>

#include "glm/glm.hpp"

#ifndef PI
#define PI  3.14159265358979323846
#endif

#ifndef TAU
#define TAU 6.28318530717958647693
#endif

namespace hilma {

inline glm::vec2 toPolar(const glm::vec2& _cartA, const glm::vec2& _cartB) {
    glm::vec2 delta = _cartB - _cartA;
    return glm::vec2(atan2(delta.y,delta.x), glm::length(glm::vec2(delta.x,delta.y)));
}

inline glm::vec2 toCartesian(const glm::vec2& _polar) {
    return glm::vec2(_polar.y*cos(_polar.x), _polar.y*sin(_polar.x));
}

inline float mapValue(const float& _value, const float& _inputMin, const float& _inputMax, const float& _outputMin, const float& _outputMax, bool _clamp) {
    if (fabs(_inputMin - _inputMax) < std::numeric_limits<float>::epsilon()) { return _outputMin; } else {
        float outVal = ((_value - _inputMin) / (_inputMax - _inputMin) * (_outputMax - _outputMin) + _outputMin);

        if (_clamp) {
            if (_outputMax < _outputMin) {
                if (outVal < _outputMax) {
                    outVal = _outputMax;
                } else if (outVal > _outputMin) {
                    outVal = _outputMin;
                }
            } else {
                if (outVal > _outputMax) {
                    outVal = _outputMax;
                } else if (outVal < _outputMin) {
                    outVal = _outputMin;
                }
            }
        }
        return outVal;
    }
}

}