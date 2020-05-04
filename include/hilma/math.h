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

/// \brief Compare two floating point types for equality.
///
/// From C++ FAQ:
///
/// Floating point arithmetic is different from real number arithmetic.
/// Never use `==` to compare two floating point numbers.
///
/// This solution is not completely symmetric, meaning it is possible for
/// `isFloatEqual(x, y) != isFloatEqual(y, x)`. From a practical
/// standpoint, this does not usually occur when the magnitudes of x and y are
/// significantly larger than epsilon, but your mileage may vary.
///
/// \sa https://isocpp.org/wiki/faq/newbie#floating-point-arith
/// \sa https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html
/// \tparam The floating point data type.
/// \param a The first floating point type variable to compare.
/// \param b The second floating point type variable to compare.
/// \returns True if `std::abs(x - y) <= std::numeric_limits<Type>::epsilon() * std::abs(x)`.
template<typename Type>
typename std::enable_if<std::is_floating_point<Type>::value, bool>::type isFloatEqual(const Type& a, const Type& b)
{
    return std::abs(a - b) <= std::numeric_limits<Type>::epsilon() * std::abs(a);
}

inline float lerp(float _start, float _stop, float _amt) {
    return _start + (_stop-_start) * _amt;
}

inline float wrap(float value, float from, float to){
    // algorithm from http://stackoverflow.com/a/5852628/599884
    if (from > to){
        std::swap(from, to);
    }
    float cycle = to - from;
    if (isFloatEqual(cycle, 0.0f)){
        return to;
    }
    return value - cycle * floor((value - from) / cycle);
}

float clamp(float value, float min, float max) {
	return value < min ? min : value > max ? max : value;
}

}