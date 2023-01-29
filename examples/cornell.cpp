#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/mesh.h"
#include "hilma/io/ply.h"
#include "hilma/io/obj.h"

int main(int argc, char **argv) {

    hilma::Mesh mesh;
    hilma::loadObj("CornellBox.obj", mesh);

    std::cout << "vertices: " << mesh.getVerticesTotal() << std::endl;
    std::cout << "normals: " << mesh.getNormalsTotal() << std::endl;
    std::cout << "colors: " << mesh.getColorsTotal() << std::endl;
    std::cout << "texcoords: " << mesh.getTexCoordsTotal() << std::endl;
    std::cout << "edges indices: " << mesh.getEdgeIndicesTotal() << std::endl;
    std::cout << "faces indices: " << mesh.getFaceIndicesTotal() << std::endl;

    std::cout << "materials: ";
    std::vector<std::string> materials_names = mesh.getMaterialsNames();
    for (size_t i = 0; i < materials_names.size(); i++)
        std::cout << materials_names[i] << " ";
    std::cout << std::endl;

    // std::cout << "triangles: ";
    // std::vector<hilma::Triangle> triangles = mesh.getTriangles();
    // std::cout << triangles.size() << std::endl;

    // if (triangles[0].material)
    //     std::cout << " mat " <<triangles[0].material->name << std::endl;

    // std::vector<hilma::Mesh> parts = mesh.getMeshesByMaterials();
    // std::cout << "Parts " << parts.size() << std::endl;
    // for (size_t i = 0; i < parts.size(); i++) {
    //     std::string name = parts[i].getMaterialsNames()[0];
    //     std::cout << " - Material:" << name << std::endl;
    //     std::cout << "   vertices:" << parts[i].getVerticesTotal() << std::endl;
    //     std::cout << "      faces:" << parts[i].getFaceIndicesTotal()/3 << std::endl;
    //     hilma::savePly(name + ".ply", parts[i], false);
    //     std::cout << std::endl;
    // }

    hilma::savePly("cornell.ply", mesh, false);
    hilma::saveObj("cornell.obj", mesh);

    return 0;
}