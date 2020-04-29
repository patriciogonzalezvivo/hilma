#pragma once

#include <string>
#include "hilma/Mesh.h"

bool savePLY( const std::string& _filename, Mesh& _mesh, bool _binnary );
// std::vector<Mesh> loadPLY( const std::string& _filename );