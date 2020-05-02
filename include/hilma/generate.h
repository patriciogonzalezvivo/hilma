#pragma once

#include "hilma/Mesh.h"

namespace hilma {

Mesh plane(float _width, float _height, int _columns, int _rows, PrimitiveMode _mode = TRIANGLES);
// Mesh cube( float _size );
// Mesh box( float width, float height, float depth, int resX, int resY, int resZ );
// Mesh sphere( float radius, int res, PrimitiveMode mode );
// Mesh icoSphere( float _radius, std::size_t _iterations );
// Mesh cylinder( float radius, float height, int radiusSegments, int heightSegments, int numCapSegments, bool bCapped, PrimitiveMode mode );
// Mesh cone( float radius, float height, int radiusSegments, int heightSegments, int capSegments, PrimitiveMode mode );

}