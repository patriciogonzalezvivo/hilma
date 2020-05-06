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
    int total = 10000;
    for (int i = 0; i < total; i++) {
        float a = glm::radians(i * 1.6f);
        polyline.addVertex(glm::vec3(cos(a*1.2), sin(a*2.), -cos(a*3.2) * sin(a*1.5)) * 10.0f);
    }
    polyline.setClosed(true);

    hilma::Mesh mesh = hilma::tube(polyline, 1.0, 18);
    hilma::savePly("tube.ply", mesh, false);

    return 1;
}