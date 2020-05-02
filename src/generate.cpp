#include "hilma/generate.h"

namespace hilma {

Mesh plane(float width, float height, int columns, int rows, PrimitiveMode mode ) {
    Mesh mesh;

    if (mode != TRIANGLE_STRIP && mode != TRIANGLES)
        mode = TRIANGLES;

    mesh.setMode(mode);

    glm::vec3 vert;
    glm::vec3 normal(0.0f, 0.0f, 1.0f); // always facing forward //
    glm::vec2 texcoord;

    // the origin of the plane is at the center //
    float halfW = width  * 0.5f;
    float halfH = height * 0.5f;
    
    // add the vertexes //
    for (int iy = 0; iy != rows; iy++) {
        for (int ix = 0; ix != columns; ix++) {

            // normalized tex coords //
            texcoord.x =       ((float)ix/((float)columns-1));
            texcoord.y = 1.f - ((float)iy/((float)rows-1));

            vert.x = texcoord.x * width - halfW;
            vert.y = -(texcoord.y-1) * height - halfH;

            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }

    if (mode == TRIANGLE_STRIP) {
        for (int y = 0; y < rows-1; y++) {
            // even rows //
            if ((y&1)==0) {
                for (int x = 0; x < columns; x++) {
                    mesh.addIndex( (y) * columns + x );
                    mesh.addIndex( (y+1) * columns + x);
                }
            } else {
                for (int x = columns-1; x >0; x--) {
                    mesh.addIndex( (y+1) * columns + x );
                    mesh.addIndex( y * columns + x-1 );
                }
            }
        }

        if (rows%2!=0) mesh.addIndex(mesh.getVerticesTotal()-columns);
    } else {

        // Triangles //
        for (int y = 0; y < rows-1; y++) {
            for (int x = 0; x < columns-1; x++) {
                // first triangle //
                mesh.addIndex((y)*columns + x);
                mesh.addIndex((y)*columns + x+1);
                mesh.addIndex((y+1)*columns + x);

                // second triangle //
                mesh.addIndex((y)*columns + x+1);
                mesh.addIndex((y+1)*columns + x+1);
                mesh.addIndex((y+1)*columns + x);
            }
        }
    }

    return mesh;
}

}