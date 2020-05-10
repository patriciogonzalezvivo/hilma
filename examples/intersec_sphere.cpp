#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Mesh.h"
#include "hilma/types/Ray.h"
#include "hilma/ops/generate.h"
#include "hilma/ops/intersection.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {
    hilma::Mesh mesh_org = hilma::icosphere(5.0, 2);
    hilma::Ray ray(glm::vec3(0.0, 0.0, -5.0), glm::vec3(0.0, 1.0, 10.0));

    std::vector<hilma::Triangle> triangles = mesh_org.getTriangles();
    for (size_t i = 0; i < triangles.size(); i++) {
        hilma::IntersectionData data = intersection(ray, triangles[i]);

        if (data.isIntersection)
            triangles[i].setColor(1.0, 0.0, 0.0);
        else
            triangles[i].setColor(1.0, 1.0, 1.0);
    }

    hilma::Mesh mesh_target;
    mesh_target.addTriangles( triangles.data(), triangles.size() );
    mesh_target.addEdge(ray);

    hilma::savePly("intersec_head.ply", mesh_target, false);

    return 1;
}