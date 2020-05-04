#pragma once

#include "hilma/types/Mesh.h"
#include "hilma/types/Polyline.h"

namespace hilma {

class Builder {
public:
    template<typename T>
    static Mesh surface(const std::vector<std::vector<T>>& _polygon);
    
    template<typename T>
    static Mesh extrude(const std::vector<std::vector<T>>& _polygon, float _maxHeight, float _minHeight = 0.0f);

    template<typename T>
    static Mesh spline(const std::vector<T>& _polyline, float _width, JoinType _join = JoinType::MITER, CapType _cap = CapType::BUTT, float _miterLimit = 3.0);// , bool _close = false);
};


// Mesh surface(const Polyline& _polyline);
template<typename T>
inline Mesh surface(const std::vector<T>& _contour) {
    std::vector<std::vector<T>> polygon;
    polygon.push_back(_contour);
    return Builder::extrude<T>(polygon);
}

template<typename T>
inline Mesh surface(const std::vector<std::vector<T>>& _polygon) {
    return Builder::surface<T>(_polygon);
}

// Mesh extrude(const Polyline& _polyline, float _maxHeight, float _minHeight = 0.0);
template<typename T>
inline Mesh extrude(const std::vector<T>& _contour, float _maxHeight, float _minHeight = 0.0) {
    std::vector<std::vector<T>> polygon;
    polygon.push_back(_contour);
    return Builder::extrude<T>(polygon, _maxHeight, _minHeight);
}

template<typename T>
inline Mesh extrude(const std::vector<std::vector<T>>& _polygon, float _maxHeight, float _minHeight = 0.0f) {
    return Builder::extrude<T>(_polygon, _maxHeight, _minHeight);
}

template<typename T>
inline Mesh spline(const std::vector<T>& _polyline, float _width, JoinType _join = JoinType::MITER, CapType _cap = CapType::BUTT, float _miterLimit = 3.0) {
    return Builder::spline<T>(_polyline, _width, _join, _cap, _miterLimit);
}

}