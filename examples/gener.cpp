#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Mesh.h"
#include "hilma/ops/generate.h"
#include "hilma/ops/transform.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {

    hilma::Mesh scene;
    

    hilma::Mesh box = hilma::box(1.0, 1.0, 1.0, 2, 2, 2);
    scene.append(box);
    
    hilma::Mesh cone = hilma::cone(0.5f, 1.f, 36, 1, 1);
    hilma::rotateX(cone, PI);
    hilma::translateX(cone, -2.0f);
    scene.append(cone);

    hilma::Mesh cylinder = hilma::cylinder(0.5f, 1.f, 36, 1, 1, true);
    hilma::translateX(cylinder, 2.0f);
    scene.append(cylinder);

    hilma::Mesh icosphere = hilma::icosphere(0.5f, 2);
    hilma::translateZ(icosphere, 2.0f);
    scene.append(icosphere);

    hilma::Mesh sphere = hilma::sphere(0.5f, 18);
    hilma::translateZ(sphere, -2.0f);
    scene.append(sphere);
    
    hilma::Mesh plane = hilma::plane(10.0f, 10.0f, 10, 10);
    hilma::rotateX(plane, -PI/2.);
    hilma::translateY(plane, -1.0f);
    scene.append(plane);

    hilma::savePly("gener.ply", scene, false);

    return 1;
}