#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Mesh.h"
#include "hilma/ops/generate.h"
#include "hilma/ops/compute.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {
    hilma::Mesh mesh = hilma::sphere(1., 18, hilma::TRIANGLE_STRIP);
    hilma::savePly("sphere.ply", mesh, false);

    hilma::Mesh mesh_smooth = hilma::getSmoothNormals(mesh, 100.);
    hilma::savePly("sphere_smooth.ply", mesh_smooth, false);

    return 1;
}