#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Mesh.h"
#include "hilma/types/Polyline.h"
#include "hilma/ops/modify.h"
#include "hilma/ops/transform.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {

    hilma::Polyline polyline;
    int total = 1000;
    for (int i = 0; i < total; i++) {
        float a = glm::radians(i * 0.5f);
        polyline.addVertex(glm::vec3(cos(a), sin(a), 0.0) * 10.0f);
    }
    polyline.close();
    
    hilma::Mesh mesh = hilma::tube(polyline, 1.0, 36, true);

    scaleZ(mesh, 2.);
    mesh.computeNormals();

    hilma::savePly("ring.ply", mesh, false);

    return 1;
}