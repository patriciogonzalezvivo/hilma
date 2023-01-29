#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/mesh.h"
#include "hilma/ops/generate.h"
#include "hilma/ops/transform.h"
#include "hilma/io/ply.h"

using namespace hilma;
using namespace glm;

int main(int argc, char **argv) {

    Mesh scene;
    
    Mesh box = hilma::box(1.0f, 1.0f, 1.0f, 2, 2, 2);
    scene.append(box);
    
    Mesh cone = hilma::cone(0.5f, 1.f, 36, 1, 1);
    rotateX(cone, PI);
    translateX(cone, -2.0f);
    scene.append(cone);

    Mesh cylinder = hilma::cylinder(0.5f, 1.f, 36, 1, 1, true);
    translateX(cylinder, 2.0f);
    scene.append(cylinder);

    Mesh icosphere = hilma::icosphere(0.5f, 2);
    translateZ(icosphere, 2.0f);
    scene.append(icosphere);

    Mesh sphere = hilma::sphere(0.5f, 18);
    translateZ(sphere, -2.0f);
    scene.append(sphere);
    
    Mesh plane = hilma::plane(10.0f, 10.0f, 10, 10);
    rotateX(plane, -PI/2.);
    translateY(plane, -1.0f);
    scene.append(plane);

    savePly("gener.ply", scene, false);

    return 1;
}