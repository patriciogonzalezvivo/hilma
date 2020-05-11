#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Mesh.h"
#include "hilma/ops/generate.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {
    hilma::Mesh mesh_src = hilma::icosphere(1.0, 6);
    hilma::Mesh mesh_dst;

    std::vector<hilma::Triangle> triangles = mesh_src.getTriangles();

    for (size_t i = 0; i < triangles.size(); i++)
        if (triangles[i].getCentroid().y > 0.25)
            mesh_dst.addTriangle(triangles[i]);
    
    mesh_dst.mergeDuplicateVertices();
    hilma::savePly("half_icosphere.ply", mesh_dst, false);

    return 1;
}