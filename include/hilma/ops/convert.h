#pragma once

#include "hilma/types/Mesh.h"
#include "hilma/types/Polyline.h"

namespace hilma {

struct Convert {
    template<typename T>
    static Mesh toSurface(const std::vector<std::vector<T>>& _polygon);
    
    template<typename T>
    static Mesh toWall(const std::vector<std::vector<T>>& _polygon, float _maxHeight, float _minHeight = 0.0f);

    template<typename T>
    static Mesh toSpline(const std::vector<T>& _polyline, float _width, JoinType _join = JoinType::MITER, CapType _cap = CapType::BUTT, float _miterLimit = 3.0);// , bool _close = false);

};

template<typename T>
inline Mesh toSurface(const std::vector<T>& _contour) {
    std::vector<std::vector<T>> polygon;
    polygon.push_back(_contour);
    return Convert::toSurface<T>(polygon);
}

inline Mesh toSurface(const Polyline& _polyline) {
    return toSurface(_polyline.getVertices());
}

template<typename T>
inline Mesh toSurface(const std::vector<std::vector<T>>& _polygon) {
    return Convert::toSurface<T>(_polygon);
}

template<typename T>
inline Mesh toWall(const std::vector<T>& _contour, float _maxHeight, float _minHeight = 0.0) {
    std::vector<std::vector<T>> polygon;
    polygon.push_back(_contour);
    return Convert::toWall<T>(polygon, _maxHeight, _minHeight);
}

inline Mesh toWall(const Polyline& _polyline, float _maxHeight, float _minHeight = 0.0)  {
    return toWall(_polyline.getVertices(), _maxHeight, _minHeight);
}

template<typename T>
inline Mesh toWall(const std::vector<std::vector<T>>& _polygon, float _maxHeight, float _minHeight = 0.0f) {
    return Convert::toWall<T>(_polygon, _maxHeight, _minHeight);
}

template<typename T>
inline Mesh toSpline(const std::vector<T>& _polyline, float _width, JoinType _join = JoinType::MITER, CapType _cap = CapType::BUTT, float _miterLimit = 3.0) {
    return Convert::toSpline<T>(_polyline, _width, _join, _cap, _miterLimit);
}

inline Mesh toSpline(const Polyline& _polyline, float _width, JoinType _join = JoinType::MITER, CapType _cap = CapType::BUTT, float _miterLimit = 3.0)  {
    return toSpline(_polyline.getVertices(), _width, _join, _cap, _miterLimit);
}


Mesh toTube(const Polyline& _polyline, const float* _array1D, int _n,  int _resolution, bool _caps);

inline Mesh toTube(const Polyline& _polyline, const float _width, int _resolution, bool _caps = true) {
    return toTube(_polyline, &_width, 1, _resolution, _caps);
}

std::vector<Line>   toLines(const BoundingBox& _bbox);
Mesh                toEdges(const BoundingBox& _bbox);

}