#pragma once

#include <string>

#include "hilma/types/Mesh.h"

namespace hilma {

Mesh    loadObj( const std::string& _filename);
bool    loadObj( const std::string& _filename, Mesh& _mesh );
bool    saveObj( const std::string& _filename, const Mesh& _mesh );

}
