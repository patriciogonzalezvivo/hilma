#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/mesh.h"

#include "hilma/ops/compute.h"

#include "hilma/io/auto.h"
#include "hilma/io/gltf.h"
#include "hilma/io/stl.h"
#include "hilma/io/obj.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {

    hilma::Mesh mesh;

    // hilma::load("head.ply", mesh);
    // hilma::load("CornellBox.obj", mesh);
    // hilma::load("BoomBox.glb", mesh);
    // hilma::load("duck.glb", mesh);
    hilma::load("dragon.obj", mesh);
    hilma::load("gazerbo.ply", mesh);
    // mesh = hilma::icosphere(1, 2);

    std::cout << "vertices: " << mesh.getVerticesTotal() << std::endl;
    std::cout << "colors: " << mesh.getColorsTotal() << std::endl;
    std::cout << "tangents: " << mesh.getTangetsTotal() << std::endl;
    std::cout << "normals: " << mesh.getNormalsTotal() << std::endl;
    std::cout << "texcoords: " << mesh.getTexCoordsTotal() << std::endl;
    std::cout << "indices: " << mesh.getFaceIndicesTotal() << std::endl;

    // mesh.flatNormals();
    
    hilma::textureAtlas( mesh );
    hilma::savePly("out.ply", mesh, false, true);

    return 1;
}