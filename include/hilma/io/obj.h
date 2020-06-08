#pragma once

#include <string>

#include "hilma/types/Mesh.h"

namespace hilma {

bool loadObj( const std::string& _filename, Mesh& _mesh );

inline Mesh loadObj( const std::string& _filename) {
    Mesh mesh;
    loadObj(_filename, mesh);
    return mesh;
}

bool saveObj( const std::string& _filename, const Mesh& _mesh );

}
