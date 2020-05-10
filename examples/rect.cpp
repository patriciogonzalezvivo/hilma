#include <string>
#include <sstream>
#include <unistd.h>
#include <iostream>

#include "hilma/types/Mesh.h"
#include "hilma/io/ply.h"

int main(int argc, char **argv) {

    int size = 1024;
    float half = size/2.0;

    hilma::Mesh mesh_A;
    float vertices_A[][2] = { {-half, -half} , {-half, half} , {half, half}, {half, -half} };
    INDEX_TYPE indices_A[][3] = { {1, 0, 3}, {1, 3, 2 } };
    mesh_A.addVertices( &vertices_A[0][0], 4, 2);
    mesh_A.addFaces( &indices_A[0][0], 2, 3);
    hilma::savePly("rect_A.ply", mesh_A, false);

    hilma::Mesh mesh_B;
    glm::vec3 vertices_B[] = { glm::vec3(-half, -half, 0.0) , glm::vec3(-half, half, 0.0) , glm::vec3(half, half, 0.0), glm::vec3(half, -half, 0.0) };
    INDEX_TYPE indices_B[] = { 1, 0, 3, 1, 3, 2  };
    mesh_B.addVertices( &vertices_B[0].x, 4, 3);
    mesh_B.addIndices( &indices_B[0], 6);
    hilma::savePly("rect_B.ply", mesh_B, false);

    hilma::Mesh mesh_C;
    mesh_C.addVertex( -half, -half );
    mesh_C.addVertex( -half, half );
    mesh_C.addVertex( half, half );
    mesh_C.addVertex( half, -half );
    mesh_C.addIndex( 1 );
    mesh_C.addIndex( 0 );
    mesh_C.addIndex( 3 );
    mesh_C.addIndex( 1 );
    mesh_C.addIndex( 3 );
    mesh_C.addIndex( 2 );
    hilma::savePly("rect_C.ply", mesh_C, false);

    hilma::Mesh mesh_D;
    mesh_D.addVertex( glm::vec3(-half, -half, 0.0) );
    mesh_D.addVertex( glm::vec3(-half, half, 0.0) );
    mesh_D.addVertex( glm::vec3(half, half, 0.0) );
    mesh_D.addVertex( glm::vec3(half, -half, 0.0) );
    mesh_D.addTriangleIndices( 1, 0, 3 );
    mesh_D.addTriangleIndices( 1, 3, 2 );
    hilma::savePly("rect_D.ply", mesh_D, false);

    hilma::Mesh mesh_E;
    mesh_E.setMode(hilma::QUAD);
    mesh_E.addVertex( glm::vec3(-half, -half, 0.0) );
    mesh_E.addVertex( glm::vec3(-half, half, 0.0) );
    mesh_E.addVertex( glm::vec3(half, half, 0.0) );
    mesh_E.addVertex( glm::vec3(half, -half, 0.0) );
    mesh_E.addQuadIndices(0, 1, 2, 3);
    hilma::savePly("rect_E.ply", mesh_E, false);

    hilma::Mesh mesh_F;
    mesh_F.setMode(hilma::TRIANGLE_STRIP);
    mesh_F.addVertex( glm::vec3(-half, -half, 0.0) );
    mesh_F.addVertex( glm::vec3(-half, half, 0.0) );
    mesh_F.addVertex( glm::vec3(half, half, 0.0) );
    mesh_F.addVertex( glm::vec3(half, -half, 0.0) );
    hilma::savePly("rect_F.ply", mesh_F, false);

    return 1;
}