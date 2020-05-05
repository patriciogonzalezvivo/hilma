#pragma once

#include "hilma/types/Mesh.h"
#include "hilma/types/Polyline.h"

namespace hilma {

class Modify {
public:
    Modify() {};
    virtual ~Modify() {};

    template<typename T>
    static Mesh surface(const std::vector<std::vector<T>>& _polygon);
    
    template<typename T>
    static Mesh extrude(const std::vector<std::vector<T>>& _polygon, float _maxHeight, float _minHeight = 0.0f);

    template<typename T>
    static Mesh spline(const std::vector<T>& _polyline, float _width, JoinType _join = JoinType::MITER, CapType _cap = CapType::BUTT, float _miterLimit = 3.0);// , bool _close = false);
};

template<typename T>
inline Mesh surface(const std::vector<T>& _contour) {
    std::vector<std::vector<T>> polygon;
    polygon.push_back(_contour);
    return Modify::surface<T>(polygon);
}

inline Mesh surface(const Polyline& _polyline) {
    return surface(_polyline.getVertices());
}

template<typename T>
inline Mesh surface(const std::vector<std::vector<T>>& _polygon) {
    return Modify::surface<T>(_polygon);
}

template<typename T>
inline Mesh extrude(const std::vector<T>& _contour, float _maxHeight, float _minHeight = 0.0) {
    std::vector<std::vector<T>> polygon;
    polygon.push_back(_contour);
    return Modify::extrude<T>(polygon, _maxHeight, _minHeight);
}

inline Mesh extrude(const Polyline& _polyline, float _maxHeight, float _minHeight = 0.0)  {
    return extrude(_polyline.getVertices(), _maxHeight, _minHeight);
}

template<typename T>
inline Mesh extrude(const std::vector<std::vector<T>>& _polygon, float _maxHeight, float _minHeight = 0.0f) {
    return Modify::extrude<T>(_polygon, _maxHeight, _minHeight);
}

template<typename T>
inline Mesh spline(const std::vector<T>& _polyline, float _width, JoinType _join = JoinType::MITER, CapType _cap = CapType::BUTT, float _miterLimit = 3.0) {
    return Modify::spline<T>(_polyline, _width, _join, _cap, _miterLimit);
}

inline Mesh spline(const Polyline& _polyline, float _width, JoinType _join = JoinType::MITER, CapType _cap = CapType::BUTT, float _miterLimit = 3.0)  {
    return spline(_polyline.getVertices(), _width, _join, _cap, _miterLimit);
}

}