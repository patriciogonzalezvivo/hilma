#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/Mesh.h"
#include "hilma/io/PlyOps.h"

int main(int argc, char **argv) {

    int size = 1024;
    float half = size/2.0;

    hilma::Mesh mesh_A;
    float vertices_A[] = {-half, -half, 0.0, -half, half, 0.0, half, half, 0.0, half, -half, 0.0 };
    int indices_A[] = { 1, 0, 3, 1, 3, 2 };
    mesh_A.addVertices( vertices_A, 12);
    mesh_A.addIndices( indices_A, 6);
    hilma::PlyOps::save("rect_A.ply", mesh_A, false);

    hilma::Mesh mesh_B;
    float vertices_B[][2] = { {-half, -half} , {-half, half} , {half, half}, {half, -half} };
    int indices_B[][3] = { {1, 0, 3}, {1, 3, 2 } };
    mesh_B.addVertices( &vertices_B[0][0], 4, 2);
    mesh_B.addIndices( &indices_B[0][0], 2, 3);
    hilma::PlyOps::save("rect_B.ply", mesh_B, false);

    hilma::Mesh mesh_C;
    glm::vec3 vertices_C[] = { glm::vec3(-half, -half, 0.0) , glm::vec3(-half, half, 0.0) , glm::vec3(half, half, 0.0), glm::vec3(half, -half, 0.0) };
    mesh_C.addVertices( &vertices_C[0].x, 4, 3);
    mesh_C.addIndices( &indices_B[0][0], 2, 3);
    hilma::PlyOps::save("rect_C.ply", mesh_C, false);

    hilma::Mesh mesh_D;
    mesh_D.addVertex( glm::vec3(-half, -half, 0.0) );
    mesh_D.addVertex( glm::vec3(-half, half, 0.0) );
    mesh_D.addVertex( glm::vec3(half, half, 0.0) );
    mesh_D.addVertex( glm::vec3(half, -half, 0.0) );
    mesh_D.addTriangle( 1, 0, 3 );
    mesh_D.addTriangle( 1, 3, 2 );
    hilma::PlyOps::save("rect_D.ply", mesh_D, false);

    hilma::Mesh mesh_E;
    mesh_E.addVertex( -half, -half );
    mesh_E.addVertex( -half, half );
    mesh_E.addVertex( half, half );
    mesh_E.addVertex( half, -half );
    mesh_E.addIndex( 1 );
    mesh_E.addIndex( 0 );
    mesh_E.addIndex( 3 );
    mesh_E.addIndex( 1 );
    mesh_E.addIndex( 3 );
    mesh_E.addIndex( 2 );
    hilma::PlyOps::save("rect_E.ply", mesh_E, false);

    return 1;
}