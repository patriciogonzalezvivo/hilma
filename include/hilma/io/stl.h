#pragma once

#include <string>

#include "hilma/types/Mesh.h"

namespace hilma {

bool loadStl( const std::string& _filename, Mesh& _mesh );

inline Mesh loadStl( const std::string& _filename) {
    Mesh mesh;
    loadStl(_filename, mesh);
    return mesh;
}

bool    saveStl( const std::string& _filename, const Mesh& _mesh, bool _binnary);

}
