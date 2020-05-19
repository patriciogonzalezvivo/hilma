#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Mesh.h"
#include "hilma/ops/compute.h"
#include "hilma/ops/convert.h"
#include "hilma/io/ply.h"
#include "hilma/io/stl.h"
#include "hilma/io/obj.h"

int main(int argc, char **argv) {

    hilma::Mesh mesh;

    hilma::loadPly("head.ply", mesh);

    std::cout << "vertices: " << mesh.getVerticesTotal() << std::endl;
    std::cout << "normals: " << mesh.getNormalsTotal() << std::endl;
    std::cout << "colors: " << mesh.getColorsTotal() << std::endl;
    std::cout << "texcoords: " << mesh.getTexCoordsTotal() << std::endl;
    std::cout << "indices: " << mesh.getFaceIndicesTotal() << std::endl;

    std::vector<hilma::Line> lines = hilma::toLines(getBoundingBox(mesh));
    mesh.addEdges(&lines[0], lines.size());

    hilma::saveObj("out.obj", mesh);

    hilma::savePly("out.ply", mesh, false);
    hilma::savePly("out_bin.ply", mesh, true);

    hilma::saveStl("out.stl", mesh, false);
    hilma::saveStl("out_bin.stl", mesh, true);

    return 1;
}