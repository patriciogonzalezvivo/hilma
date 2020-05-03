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

}