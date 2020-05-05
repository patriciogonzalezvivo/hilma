#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Mesh.h"
#include "hilma/ops/modify.h"
#include "hilma/ops/transform.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {

    std::vector<std::vector<glm::vec2>> polygon;
    // Fill polygon structure with actual data. Any winding order works.
    // The first polyline defines the main polygon.
    polygon.push_back({{100, 0}, {100, 100}, {0, 100}, {0, 0}, {100, 0}});
    // Following polylines define holes.
    polygon.push_back({{75, 25}, {75, 75}, {25, 75}, {25, 25}, {75, 25}});

    // hilma::Polyline p;
    // p.addVertices(&polygon[0].x, 5, 2);

    hilma::Mesh bottom = hilma::surface(polygon);
    hilma::Mesh top = bottom;
    bottom.invertWindingOrder();
    hilma::translateZ(top, 10.0f);
    hilma::Mesh walls = hilma::extrude(polygon, 10.0f);


    hilma::Mesh mesh;
    mesh.append(bottom);
    mesh.append(top);
    mesh.append(walls);
    mesh.mergeDuplicateVertices();
    hilma::savePly("extrude.ply", mesh, false);

    return 1;
}