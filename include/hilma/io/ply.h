#pragma once

#include <string>

#include "hilma/types/Mesh.h"

namespace hilma {


Mesh    loadPly( const std::string& _filename);
bool    loadPly( const std::string& _filename, Mesh& _mesh );
bool    savePly( const std::string& _filename, Mesh& _mesh, bool _binnary, bool _colorAsChar = false );

}
