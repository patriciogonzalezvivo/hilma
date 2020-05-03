#pragma once

#include "hilma/types/Mesh.h"

namespace hilma {


Mesh plane(float _width, float _height, int _columns, int _rows, PrimitiveMode _mode = TRIANGLES);
Mesh box( float _width, float _height, float _depth, int _resX, int _resY, int _resZ );
Mesh cube( float _size, int _resolution );
Mesh sphere( float _radius, int _resolution, PrimitiveMode mode = TRIANGLES);
Mesh icosphere( float _radius, std::size_t _iterations );
// Mesh cylinder( float radius, float height, int radiusSegments, int heightSegments, int numCapSegments, bool bCapped, PrimitiveMode mode );
// Mesh cone( float radius, float height, int radiusSegments, int heightSegments, int capSegments, PrimitiveMode mode );

Mesh surface(const std::vector<glm::vec2>& _contour);
Mesh surface(const std::vector<std::vector<glm::vec2>>& _polygon);

Mesh extrudeWall(const std::vector<glm::vec2>& _contour, float _maxHeight, float _minHeight = 0.0);
Mesh extrudeWall(const std::vector<std::vector<glm::vec2>>& _polygon, float _maxHeight, float _minHeight = 0.0f);

enum class CapType {
    BUTT = 0,   // No points added to end of line
    SQUARE = 2, // Two points added to make a square extension
    ROUND = 6   // Six points added in a fan to make a round cap
};

enum class JoinType {
    MITER = 0,  // No points added at line join
    BEVEL = 1,  // One point added to flatten the corner of a join
    ROUND = 5   // Five points added in a fan to make a round outer join
};

Mesh spline(const std::vector<glm::vec2>& _polyline, float _width, JoinType _join = JoinType::MITER, CapType _cap = CapType::BUTT, float _miterLimit = 3.0);// , bool _close = false);

}