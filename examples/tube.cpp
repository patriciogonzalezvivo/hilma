#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/mesh.h"
#include "hilma/types/polyline.h"
#include "hilma/ops/path.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {

    hilma::Polyline polyline;
    int total = 1000;
    for (int i = 0; i < total; i++) {
        float a = glm::radians(i * 0.5f);
        polyline.addVertex(glm::vec3(cos(a*1.2), sin(a*2.), -cos(a*3.2) * sin(a*1.5)) * 10.0f);
    }

    float width[] = {1.f, 1.1f, 1.2f, 1.1f, 1.f, 1.1f, 1.2f, 1.3f, 1.2f, 1.1f,
                     1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f };
    hilma::Mesh mesh = hilma::toTube(polyline, width, 20, 18, true);

    mesh.computeNormals();
    hilma::savePly("tube.ply", mesh, false);

    return 1;
}