#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Mesh.h"
#include "hilma/types/Polyline.h"
#include "hilma/ops/modify.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {

    hilma::Polyline polyline;
    for (int i = 0; i < 1000; i++) {
        float a = glm::radians(i/360.0f);
        polyline.addVertex(glm::vec3(cos(a*3.)*10., sin(a*2.)*6., cos(a*3.2) * sin(a*1.2) * 10.));
    }
    // polyline.setClosed(true);

    hilma::Mesh mesh = hilma::tube(polyline, 1.0, 12);
    hilma::savePly("tube.ply", mesh, false);

    return 1;
}