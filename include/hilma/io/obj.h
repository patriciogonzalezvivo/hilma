#pragma once

#include <string>

#include "hilma/Mesh.h"

namespace hilma {

// std::vector<Mesh> loadObj( const std::string& _filename );
bool saveObj( const std::string& _filename, const Mesh& _mesh );

}
