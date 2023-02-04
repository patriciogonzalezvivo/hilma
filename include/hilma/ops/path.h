#pragma once

// #include "hilma/types/mesh.h"
#include "hilma/types/polygon.h"

namespace hilma {

Mesh toSurface(const std::vector<std::vector<glm::vec3>>& _polygon, const BoundingBox& _bbox);

inline Mesh toSurface(const std::vector<std::vector<glm::vec3>>& _polygon) {
    return toSurface(_polygon, BoundingBox());
};

inline Mesh toSurface(const Polyline& _polyline, const BoundingBox& _bbox) {
    return toSurface(_polyline.getVertices(), _bbox);
}

inline Mesh toSurface(const Polyline& _polyline) {
    return toSurface(_polyline.getVertices());
}

inline Mesh toSurface(const Polygon& _polygon, const BoundingBox& _bbox) {
    std::vector< std::vector<glm::vec3> > polygon;
    for (size_t i = 0; i < _polygon.size(); i++)
        polygon.push_back( _polygon[i].getVertices() );
    
    return toSurface(polygon, _bbox);
}

inline Mesh toSurface(const Polygon& _polygon) {
    return toSurface(_polygon, BoundingBox());
}

inline Mesh toSurface(const std::vector<Polyline>& _polygon, const BoundingBox& _bbox) {
    return toSurface( Polygon(_polygon), _bbox );
}

inline Mesh toSurface(const std::vector<Polyline>& _polygon) {
    return toSurface( Polygon(_polygon), BoundingBox() );
}

Mesh toWall(const std::vector<glm::vec3>& _polyline, float _maxHeight, float _minHeight = 0.0f);

inline Mesh toWall(const Polyline& _polyline, float _maxHeight, float _minHeight = 0.0)  {
    return toWall(_polyline.getVertices(), _maxHeight, _minHeight);
}

inline Mesh toWall(const Polygon& _polygon, float _maxHeight, float _minHeight = 0.0f) {
    Mesh mesh;
    for (size_t i = 0; i < _polygon.size(); i++)
        mesh.append( toWall( _polygon[i], _maxHeight, _minHeight ) );
    return mesh;
}

Mesh toSpline(const std::vector<glm::vec3>& _polyline, float _width, JoinType _join = JoinType::MITER, CapType _cap = CapType::BUTT, float _miterLimit = 3.0);

inline Mesh toSpline(const Polyline& _polyline, float _width, JoinType _join = JoinType::MITER, CapType _cap = CapType::BUTT, float _miterLimit = 3.0)  {
    return toSpline(_polyline.getVertices(), _width, _join, _cap, _miterLimit);
}

Mesh toTube(const Polyline& _polyline, const float* _array1D, int _n,  int _resolution, bool _caps);

inline Mesh toTube(const Polyline& _polyline, const float _width, int _resolution, bool _caps = true) {
    return toTube(_polyline, &_width, 1, _resolution, _caps);
}

std::vector<Line>   toLines(const std::vector<Triangle>& _triangles);
std::vector<Line>   toLines(const BoundingBox& _bbox);

}