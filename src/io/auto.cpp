#include "hilma/io/gltf.h"

#include "hilma/io/auto.h"
#include "hilma/io/jpg.h"
#include "hilma/io/png.h"
#include "hilma/io/hdr.h"

#include "hilma/io/auto.h"

#include "hilma/fs.h"

#include "hilma/io/gltf.h"
#include "hilma/io/obj.h"
#include "hilma/io/ply.h"
#include "hilma/io/stl.h"

namespace hilma {

bool load(const std::string& _filename, Mesh& _mesh) {

    std::string ext = getExt(_filename);

    if (ext == "gltf" || ext == "GLTF" ||
        ext == "glb" || ext == "GLB" )
        return loadGltf(_filename, _mesh);
    else if (ext == "obj" || ext == "OBJ")
        return loadObj(_filename, _mesh);
    else if (ext == "ply" || ext == "PLY")
        return loadPly(_filename, _mesh);
    else if (ext == "stl" || ext == "STL")
        return loadStl(_filename, _mesh);

    return false;
}

}