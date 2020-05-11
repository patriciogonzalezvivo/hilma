#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Mesh.h"
#include "hilma/ops/generate.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {
    hilma::Mesh mesh = hilma::icosphere(1.0, 2);
    hilma::savePly("icosphere.ply", mesh, false);

    return 1;
}