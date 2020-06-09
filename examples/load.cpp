#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Mesh.h"

#include "hilma/ops/compute.h"
#include "hilma/ops/convert_path.h"
#include "hilma/ops/convert_image.h"
#include "hilma/ops/generate.h"

#include "hilma/io/auto.h"
#include "hilma/io/gltf.h"
#include "hilma/io/stl.h"
#include "hilma/io/obj.h"
#include "hilma/io/ply.h"
#include "hilma/io/png.h"

#include "hilma/timer.h"
#include "hilma/text.h"

int main(int argc, char **argv) {

    hilma::Mesh mesh;

    // hilma::load("head.ply", mesh);
    hilma::load("CornellBox.obj", mesh);
    // hilma::load("dragon.obj", mesh);
    // mesh = hilma::icosphere(1, 2);

    std::cout << "vertices: " << mesh.getVerticesTotal() << std::endl;
    std::cout << "colors: " << mesh.getColorsTotal() << std::endl;
    std::cout << "tangents: " << mesh.getTangetsTotal() << std::endl;
    std::cout << "normals: " << mesh.getNormalsTotal() << std::endl;
    std::cout << "texcoords: " << mesh.getTexCoordsTotal() << std::endl;
    std::cout << "indices: " << mesh.getFaceIndicesTotal() << std::endl;

    // std::vector<hilma::Line> lines = hilma::toLines(getBoundingBox(mesh));
    // mesh.addEdges(&lines[0], lines.size());

    // hilma::saveObj("out.obj", mesh);

    // hilma::savePly("out.ply", mesh, false);
    // hilma::savePly("out_bin.ply", mesh, true);

    // hilma::saveStl("out.stl", mesh, false);
    // hilma::saveStl("out_bin.stl", mesh, true);

    hilma::saveGltf("out.glb", mesh);
    hilma::saveGltf("out.gltf", mesh);

    // hilma::Timer timer;
    // timer.start();
    // std::vector<hilma::Image> sdf = hilma::toSdf(mesh, 5.0f, true);
    // timer.stop();

    // const float seconds = timer.get() / 1000.f;
    // std::cout << " Processing time : " << seconds << " secs" << std::endl;

    // for (size_t i = 0; i < sdf.size(); i++)
    //     hilma::savePng("sdf_" + hilma::toString(i, 4, '0') + ".png", sdf[i]);

    return 1;
}