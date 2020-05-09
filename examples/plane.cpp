#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Mesh.h"
#include "hilma/ops/generate.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {
    int size = 1024;

    hilma::Mesh mesh = hilma::plane(1024, 1024, 10, 10, hilma::TRIANGLE_STRIP);

    hilma::savePly("plane.ply", mesh, false);

    return 1;
}