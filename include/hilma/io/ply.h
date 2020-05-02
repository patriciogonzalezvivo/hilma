#pragma once

#include <string>

#include "hilma/Mesh.h"

namespace hilma {

bool loadPly( const std::string& _filename, Mesh& _mesh );
bool savePly( const std::string& _filename, Mesh& _mesh, bool _binnary );

}
