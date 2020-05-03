#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Mesh.h"
#include "hilma/ops/modify.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {

    std::vector<std::vector<glm::vec2>> polygon;
    // Fill polygon structure with actual data. Any winding order works.
    // The first polyline defines the main polygon.
    polygon.push_back({{100, 0}, {100, 100}, {0, 100}, {0, 0}});
    // Following polylines define holes.
    polygon.push_back({{75, 25}, {75, 75}, {25, 75}, {25, 25}});

    hilma::Mesh mesh = hilma::surface(polygon);
    hilma::savePly("surface.ply", mesh, false);

    return 1;
}