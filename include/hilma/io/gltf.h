#pragma once

#include <string>

#include "hilma/types/Mesh.h"

namespace hilma {

// Mesh    loadGltf( const std::string& _filename);
bool    loadGltf( const std::string& _filename, Mesh& _mesh );
// bool    saveGltf( const std::string& _filename, const Mesh& _mesh );

}
