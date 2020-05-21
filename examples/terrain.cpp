#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Image.h"
#include "hilma/ops/convert.h"
#include "hilma/ops/transform.h"
#include "hilma/io/png.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {

    hilma::Image heightmap;
    loadPng("gale.png", heightmap, 1);

    hilma::Mesh mesh = hilma::toTerrain(heightmap, 100.0, 0.05, 0.5);
    hilma::scale(mesh, 1.0/heightmap.getWidth());
    savePly("gale.ply", mesh, false); 

    return 1;
}