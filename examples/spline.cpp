#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Mesh.h"
#include "hilma/ops/convert_path.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {

    std::vector<glm::vec3> lineA = {{100, 0, 0}, {100, 100, 0}, {0, 100, 0}, {0, 0, 0}};
    std::vector<glm::vec2> lineB = {{75, 25}, {75, 75}, {25, 75}, {25, 25}};

    hilma::Mesh mesh;
    mesh.append(hilma::toSpline( lineA, 3, hilma::JoinType::ROUND, hilma::CapType::ROUND));
    mesh.append(hilma::toSpline( hilma::Polyline( lineB ), 3));
    hilma::savePly("spline.ply", mesh, false);

    return 1;
}