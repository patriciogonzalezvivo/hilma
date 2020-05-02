#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/Mesh.h"
#include "hilma/generate.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {
    int size = 1024;

    hilma::Mesh mesh = hilma::icosphere(size/2, 2);
    mesh.invertWindingOrder();
    hilma::savePly("icosphere.ply", mesh, false);

    return 1;
}