#pragma once

#include <math.h>
#include <cstdlib>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"
#include "glm/glm.hpp"

#ifndef PI
#define PI  3.14159265358979323846
#endif

#ifndef TAU
#define TAU 6.28318530717958647693
#endif

#ifndef EPS
#define EPS 1.0E-10
#endif

namespace hilma {

// Returns a random real in [0,1).
inline float randomf() {
    return rand() / (RAND_MAX + 1.0f);
}

// Returns a random real in [min,max).
inline float randomf(float _min, float _max) {
    return _min + (_max-_min) * randomf();
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
    float r = std::sqrt(1.0f - z*z);
    return glm::vec3(r*cos(a), r*sin(a), z);
}

inline glm::vec3 random_in_hemisphere(const glm::vec3& normal) {
    glm::vec3 in_unit_sphere = random_in_unit_sphere() ;
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

inline glm::vec3 random_in_unit_disk() {
    while (true) {
        glm::vec3 p = glm::vec3(randomf(-1.0f,1.0f), randomf(-1.0f,1.0f), 0.0f);
        if (glm::length2(p) >= 1.0f) continue;
        return p;
    }
}

inline float remap(const float& _value, const float& _inputMin, const float& _inputMax, const float& _outputMin, const float& _outputMax, bool _clamp) {
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

inline float clamp(float value, float min, float max) {
	return value < min ? min : value > max ? max : value;
}

inline float saturate(float value) { 
    return std::fmax (0.0f, std::fmin (1.0f, value)); 
}

template <class T>
inline int sign(const T &x) { return (x >= 0 ? 1 : -1); };


template <class T>
inline T square(const T &x) { return x*x; };

}