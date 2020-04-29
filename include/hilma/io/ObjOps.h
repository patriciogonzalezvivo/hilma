#pragma once

#include <string>

#include "hilma/Mesh.h"

namespace hilma {

class ObjOps {

    static bool save( const std::string& _filename, const Mesh& _mesh );
    // std::vector<Mesh> loadOBJ( const std::string& _filename );
};

}
