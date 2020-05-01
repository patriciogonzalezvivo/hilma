#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/Mesh.h"
#include "hilma/io/PlyOps.h"

int main(int argc, char **argv) {

    hilma::Mesh mesh;

    hilma::PlyOps::load("cube.ply", mesh);

    std::cout << "vertices: " << mesh.getVerticesTotal() << std::endl;
    std::cout << "normals: " << mesh.getNormalsTotal() << std::endl;
    std::cout << "colors: " << mesh.getColorsTotal() << std::endl;
    std::cout << "texcoords: " << mesh.getTexCoordsTotal() << std::endl;
    std::cout << "indices: " << mesh.getIndicesTotal() << std::endl;

    hilma::PlyOps::save("out.ply", mesh, false);

    return 1;
}