#pragma once

#include <string>

#include "hilma/Mesh.h"

namespace hilma {

class PlyOps {
public:

    static bool save( const std::string& _filename, Mesh& _mesh, bool _binnary );
    // static std::vector<Mesh> loadPLY( const std::string& _filename );

};

}
