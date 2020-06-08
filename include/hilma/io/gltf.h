#pragma once

#include <string>

#include "hilma/types/Mesh.h"

namespace hilma {

bool loadGltf( const std::string& _filename, Mesh& _mesh );

inline Mesh loadGltf( const std::string& _filename) {
    Mesh mesh;
    loadGltf(_filename, mesh);
    return mesh;
}

bool saveGltf( const std::string& _filename, const Mesh& _mesh );

}
