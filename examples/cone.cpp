#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Mesh.h"
#include "hilma/ops/generate.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {

    hilma::Mesh mesh = hilma::cone(1.f, 2.f, 36, 1, 1);
    hilma::savePly("cone.ply", mesh, false);

    return 1;
}