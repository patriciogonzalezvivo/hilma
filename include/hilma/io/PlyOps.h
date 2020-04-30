#pragma once

#include <string>

#include "hilma/Mesh.h"

namespace hilma {

class PlyOps {
public:

    static bool load( const std::string& _filename, Mesh& _mesh );
    static bool save( const std::string& _filename, Mesh& _mesh, bool _binnary );

};

}
