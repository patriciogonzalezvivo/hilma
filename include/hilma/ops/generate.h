#pragma once

#include "hilma/types/Mesh.h"

namespace hilma {

Mesh plane(float _width, float _height, int _columns, int _rows, FaceType _mode = TRIANGLES);
Mesh box( float _width, float _height, float _depth, int _resX, int _resY, int _resZ );
Mesh cube( float _size, int _resolution );
Mesh sphere( float _radius, int _resolution, FaceType mode = TRIANGLES);
Mesh icosphere( float _radius, size_t _iterations );
Mesh cylinder( float _radius, float _height, int _radiusSegments, int _heightSegments, int _numCapSegments, bool _bCapped, FaceType _mode = TRIANGLES);
Mesh cone( float _radius, float _height, int _radiusSegments, int _heightSegments, int _capSegments, FaceType _mode = TRIANGLES);

}