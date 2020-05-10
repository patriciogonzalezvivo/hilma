#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Mesh.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {

    hilma::Mesh mesh;

    hilma::loadPly("cube.ply", mesh);

    std::cout << "vertices: " << mesh.getVerticesTotal() << std::endl;
    std::cout << "normals: " << mesh.getNormalsTotal() << std::endl;
    std::cout << "colors: " << mesh.getColorsTotal() << std::endl;
    std::cout << "texcoords: " << mesh.getTexCoordsTotal() << std::endl;
    std::cout << "indices: " << mesh.getFaceIndicesTotal() << std::endl;

    hilma::savePly("out.ply", mesh, false);

    return 1;
}