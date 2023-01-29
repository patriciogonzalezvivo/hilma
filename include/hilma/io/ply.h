#pragma once

#include <string>

#include "hilma/types/mesh.h"

namespace hilma {

bool loadPly( const std::string& _filename, Mesh& _mesh );

inline Mesh loadPly( const std::string& _filename) {
    Mesh mesh;
    loadPly(_filename, mesh);
    return mesh;
}

bool savePly( const std::string& _filename, Mesh& _mesh, bool _binnary, bool _colorAsChar = false );

}
