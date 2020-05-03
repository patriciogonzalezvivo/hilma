#include "hilma/ops/generate.h"
#include "hilma/ops/earcut.h"

#include "hilma/math.h"
#include "hilma/types/BoundingBox.h"

namespace hilma {

Mesh plane(float _width, float _height, int _columns, int _rows, PrimitiveMode _mode ) {
    Mesh mesh;

    if (_mode != TRIANGLE_STRIP && _mode != TRIANGLES)
        _mode = TRIANGLES;

    mesh.setMode(_mode);

    glm::vec3 vert;
    glm::vec3 normal(0.0f, 0.0f, 1.0f); // always facing forward //
    glm::vec2 texcoord;

    // the origin of the plane is at the center //
    float halfW = _width  * 0.5f;
    float halfH = _height * 0.5f;
    
    // add the vertexes //
    for (int iy = 0; iy != _rows; iy++) {
        for (int ix = 0; ix != _columns; ix++) {

            // normalized tex coords //
            texcoord.x =       ((float)ix/((float)_columns-1));
            texcoord.y = 1.f - ((float)iy/((float)_rows-1));

            vert.x = texcoord.x * _width - halfW;
            vert.y = -(texcoord.y-1) * _height - halfH;

            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }

    if (_mode == TRIANGLE_STRIP) {
        for (int y = 0; y < _rows-1; y++) {
            // even _rows //
            if ((y&1)==0) {
                for (int x = 0; x < _columns; x++) {
                    mesh.addIndex( (y) * _columns + x );
                    mesh.addIndex( (y+1) * _columns + x);
                }
            }
            else {
                for (int x = _columns-1; x >0; x--) {
                    mesh.addIndex( (y+1) * _columns + x );
                    mesh.addIndex( y * _columns + x-1 );
                }
            }
        }

        if (_rows%2!=0) mesh.addIndex(mesh.getVerticesTotal()-_columns);
    }
    else {

        // Triangles //
        for (int y = 0; y < _rows-1; y++) {
            for (int x = 0; x < _columns-1; x++) {
                // first triangle //
                mesh.addIndex((y)*_columns + x);
                mesh.addIndex((y)*_columns + x+1);
                mesh.addIndex((y+1)*_columns + x);

                // second triangle //
                mesh.addIndex((y)*_columns + x+1);
                mesh.addIndex((y+1)*_columns + x+1);
                mesh.addIndex((y+1)*_columns + x);
            }
        }
    }

    return mesh;
}

Mesh box( float _width, float _height, float depth, int resX, int resY, int resZ ) {
    // mesh only available as triangles //
    Mesh mesh;
    mesh.setMode( TRIANGLES );

    resX = resX + 1;
    resY = resY + 1;
    resZ = resZ + 1;

    if ( resX < 2 ) resX = 0;
    if ( resY < 2 ) resY = 0;
    if ( resZ < 2 ) resZ = 0;

    // halves //
    float halfW = _width * .5f;
    float halfH = _height * .5f;
    float halfD = depth * .5f;

    glm::vec3 vert;
    glm::vec3 normal;
    glm::vec2 texcoord;
    std::size_t vertOffset = 0;

    // TRIANGLES //

    // Front Face //
    normal = {0.f, 0.f, 1.f};
    // add the vertexes //
    for (int iy = 0; iy < resY; iy++) {
        for (int ix = 0; ix < resX; ix++) {

            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = 1.f - ((float)iy/((float)resY-1.f));

            vert.x = texcoord.x * _width - halfW;
            vert.y = -(texcoord.y-1.f) * _height - halfH;
            vert.z = halfD;

            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }

    for (int y = 0; y < resY-1; y++) {
        for (int x = 0; x < resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resX + x + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);

            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
        }
    }

    vertOffset = mesh.getVerticesTotal();

    // Right Side Face //
    normal = {1.f, 0.f, 0.f};
    // add the vertexes //
    for (int iy = 0; iy < resY; iy++) {
        for (int ix = 0; ix < resZ; ix++) {

            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resZ-1.f));
            texcoord.y = 1.f - ((float)iy/((float)resY-1.f));

            //vert.x = texcoord.x * _width - halfW;
            vert.x = halfW;
            vert.y = -(texcoord.y-1.f) * _height - halfH;
            vert.z = texcoord.x * -depth + halfD;

            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }

    for (int y = 0; y < resY-1; y++) {
        for (int x = 0; x < resZ-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resZ + x + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
            mesh.addIndex((y)*resZ + x+1 + vertOffset);

            // second triangle //
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
            mesh.addIndex((y+1)*resZ + x+1 + vertOffset);
        }
    }

    vertOffset = mesh.getVerticesTotal();

    // Left Side Face //
    normal = {-1.f, 0.f, 0.f};
    // add the vertexes //
    for (int iy = 0; iy < resY; iy++) {
        for (int ix = 0; ix < resZ; ix++) {

            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resZ-1.f));
            texcoord.y = 1.f-((float)iy/((float)resY-1.f));

            //vert.x = texcoord.x * _width - halfW;
            vert.x = -halfW;
            vert.y = -(texcoord.y-1.f) * _height - halfH;
            vert.z = texcoord.x * depth - halfD;

            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }

    for (int y = 0; y < resY-1; y++) {
        for (int x = 0; x < resZ-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resZ + x + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
            mesh.addIndex((y)*resZ + x+1 + vertOffset);

            // second triangle //
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
            mesh.addIndex((y+1)*resZ + x+1 + vertOffset);
        }
    }

    vertOffset = mesh.getVerticesTotal();

    // Back Face //
    normal = {0.f, 0.f, -1.f};
    // add the vertexes //
    for (int iy = 0; iy < resY; iy++) {
        for (int ix = 0; ix < resX; ix++) {

            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = 1.f-((float)iy/((float)resY-1.f));

            vert.x = texcoord.x * -_width + halfW;
            vert.y = -(texcoord.y-1.f) * _height - halfH;
            vert.z = -halfD;

            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }

    for (int y = 0; y < resY-1; y++) {
        for (int x = 0; x < resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resX + x + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);

            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
        }
    }

    vertOffset = mesh.getVerticesTotal();


    // Top Face //
    normal = {0.f, -1.f, 0.f};
    // add the vertexes //
    for (int iy = 0; iy < resZ; iy++) {
        for (int ix = 0; ix < resX; ix++) {

            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = 1.f-((float)iy/((float)resZ-1.f));

            vert.x = texcoord.x * _width - halfW;
            //vert.y = -(texcoord.y-1.f) * _height - halfH;
            vert.y = -halfH;
            vert.z = texcoord.y * depth - halfD;

            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }

    for (int y = 0; y < resZ-1; y++) {
        for (int x = 0; x < resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);

            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
        }
    }

    vertOffset = mesh.getVerticesTotal();

    // Bottom Face //
    normal = {0.f, 1.f, 0.f};
    // add the vertexes //
    for (int iy = 0; iy < resZ; iy++) {
        for (int ix = 0; ix < resX; ix++) {

            // normalized tex coords //
            texcoord.x = ((float)ix/((float)resX-1.f));
            texcoord.y = 1.f-((float)iy/((float)resZ-1.f));

            vert.x = texcoord.x * _width - halfW;
            //vert.y = -(texcoord.y-1.f) * _height - halfH;
            vert.y = halfH;
            vert.z = texcoord.y * -depth + halfD;

            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }

    for (int y = 0; y < resZ-1; y++) {
        for (int x = 0; x < resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);

            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
        }
    }

    return mesh;
}

Mesh cube( float _size, int _resolution) {
    return box(_size, _size, _size, _resolution, _resolution, _resolution);
}

Mesh sphere( float radius, int res, PrimitiveMode _mode ) {
    Mesh mesh;

    float doubleRes = res*2.f;
    float polarInc = PI/(res); // ringAngle
    float azimInc = TAU/(doubleRes); // segAngle //

    if (_mode != TRIANGLE_STRIP && _mode != TRIANGLES)
        _mode = TRIANGLE_STRIP;
    mesh.setMode(_mode);

    glm::vec3 vert;
    glm::vec2 tcoord;

    for (float i = 0; i < res+1; i++) {

        float tr = sin( PI-i * polarInc );
        float ny = cos( PI-i * polarInc );

        tcoord.y = 1.f - (i / res);

        for (float j = 0; j <= doubleRes; j++) {

            float nx = tr * sin(j * azimInc);
            float nz = tr * cos(j * azimInc);

            tcoord.x = j / (doubleRes);

            vert = {nx, ny, nz};
            mesh.addNormal(vert);

            vert *= radius;
            mesh.addVertex(vert);
            mesh.addTexCoord(tcoord);
        }
    }

    int nr = doubleRes+1;
    if (_mode == TRIANGLES) {

        int index1, index2, index3;

        for (float iy = 0; iy < res; iy++) {
            for (float ix = 0; ix < doubleRes; ix++) {

                // first tri //
                if(iy > 0) {
                    index1 = (iy+0) * (nr) + (ix+0);
                    index2 = (iy+0) * (nr) + (ix+1);
                    index3 = (iy+1) * (nr) + (ix+0);

                    mesh.addIndex(index1);
                    mesh.addIndex(index3);
                    mesh.addIndex(index2);
                }

                if(iy < res-1 ) {
                    // second tri //
                    index1 = (iy+0) * (nr) + (ix+1);
                    index2 = (iy+1) * (nr) + (ix+1);
                    index3 = (iy+1) * (nr) + (ix+0);

                    mesh.addIndex(index1);
                    mesh.addIndex(index3);
                    mesh.addIndex(index2);

                }
            }
        }

    }
    else {
        for (int y = 0; y < res; y++) {
            for (int x = 0; x <= doubleRes; x++) {
                mesh.addIndex( (y)*nr + x );
                mesh.addIndex( (y+1)*nr + x );
            }
        }
    }


    return mesh;
}

Mesh icosphere(float radius, std::size_t iterations) {

    /// Step 1 : Generate icosahedron
    const float sqrt5 = sqrt(5.0f);
    const float phi = (1.0f + sqrt5) * 0.5f;
    const float invnorm = 1/sqrt(phi*phi+1);

    std::vector<glm::vec3> vertices;
    vertices.push_back(invnorm * glm::vec3(-1,  phi, 0));//0
    vertices.push_back(invnorm * glm::vec3( 1,  phi, 0));//1
    vertices.push_back(invnorm * glm::vec3(0,   1,  -phi));//2
    vertices.push_back(invnorm * glm::vec3(0,   1,   phi));//3
    vertices.push_back(invnorm * glm::vec3(-phi,0,  -1));//4
    vertices.push_back(invnorm * glm::vec3(-phi,0,   1));//5
    vertices.push_back(invnorm * glm::vec3( phi,0,  -1));//6
    vertices.push_back(invnorm * glm::vec3( phi,0,   1));//7
    vertices.push_back(invnorm * glm::vec3(0,   -1, -phi));//8
    vertices.push_back(invnorm * glm::vec3(0,   -1,  phi));//9
    vertices.push_back(invnorm * glm::vec3(-1,  -phi,0));//10
    vertices.push_back(invnorm * glm::vec3( 1,  -phi,0));//11
       
    std::vector<int> indices = {
        0,1,2,
        0,3,1,
        0,4,5,
        1,7,6,
        1,6,2,
        1,3,7,
        0,2,4,
        0,5,3,
        2,6,8,
        2,8,4,
        3,5,9,
        3,9,7,
        11,6,7,
        10,5,4,
        10,4,8,
        10,9,5,
        11,8,6,
        11,7,9,
        10,8,11,
        10,11,9
    };

    int size = indices.size();

    /// Step 2 : tessellate
    for (int iteration = 0; iteration < iterations; iteration++) {
        size*=4;
        std::vector<int> newFaces;
        for (int i = 0; i < size/12; i++) {
            int i1 = indices[i*3];
            int i2 = indices[i*3+1];
            int i3 = indices[i*3+2];
            size_t i12 = vertices.size();
            size_t i23 = i12+1;
            size_t i13 = i12+2;
            glm::vec3 v1 = vertices[i1];
            glm::vec3 v2 = vertices[i2];
            glm::vec3 v3 = vertices[i3];
            //make 1 vertice at the center of each edge and project it onto the sphere
            vertices.push_back(glm::normalize(v1+v2));
            vertices.push_back(glm::normalize(v2+v3));
            vertices.push_back(glm::normalize(v1+v3));
            //now recreate indices
            newFaces.push_back(i1);
            newFaces.push_back(i12);
            newFaces.push_back(i13);
            newFaces.push_back(i2);
            newFaces.push_back(i23);
            newFaces.push_back(i12);
            newFaces.push_back(i3);
            newFaces.push_back(i13);
            newFaces.push_back(i23);
            newFaces.push_back(i12);
            newFaces.push_back(i23);
            newFaces.push_back(i13);
        }
        indices.swap(newFaces);
    }

    /// Step 3 : generate texcoords
    std::vector<glm::vec2> texCoords;
    for (int i=0; i<vertices.size(); i++) {
        const auto& vec = vertices[i];
        float u, v;
        float r0 = sqrtf(vec.x*vec.x+vec.z*vec.z);
        float alpha;
        alpha = atan2f(vec.z,vec.x);
        u = alpha/TAU+.5f;
        v = atan2f(vec.y, r0)/PI + .5f;
        // reverse the u coord, so the default is texture mapped left to
        // right on the outside of a sphere 
        // reverse the v coord, so that texture origin is at top left
        texCoords.push_back(glm::vec2(1.0-u,1.f-v));
    }

    /// Step 4 : fix texcoords
    // find vertices to split
    std::vector<int> indexToSplit;

    for (int i=0; i<indices.size()/3; i++) {
        glm::vec2 t0 = texCoords[indices[i*3+0]];
        glm::vec2 t1 = texCoords[indices[i*3+1]];
        glm::vec2 t2 = texCoords[indices[i*3+2]];

        if (std::abs(t2.x-t0.x)>0.5) {
            if (t0.x<0.5)
                indexToSplit.push_back(indices[i*3]);
            else
                indexToSplit.push_back(indices[i*3+2]);
        }
        if (std::abs(t1.x-t0.x)>0.5) {
            if (t0.x<0.5)
                indexToSplit.push_back(indices[i*3]);
            else
                indexToSplit.push_back(indices[i*3+1]);
        }
        if (std::abs(t2.x-t1.x)>0.5) {
            if (t1.x<0.5)
                indexToSplit.push_back(indices[i*3+1]);
            else
                indexToSplit.push_back(indices[i*3+2]);
        }
    }

    //split vertices
    for (int i = 0; i < indexToSplit.size(); i++) {
        int index = indexToSplit[i];
        //duplicate vertex
        glm::vec3 v = vertices[index];
        glm::vec2 t = texCoords[index] + glm::vec2(1.f, 0.f);
        vertices.push_back(v);
        texCoords.push_back(t);
        int newIndex = vertices.size()-1;
        //reassign indices
        for (int j = 0; j<indices.size(); j++) {
            if (indices[j] == index) {
                int index1 = indices[(j+1)%3+(j/3)*3];
                int index2 = indices[(j+2)%3+(j/3)*3];
                if ((texCoords[index1].x>0.5) || (texCoords[index2].x>0.5))
                    indices[j] = newIndex;
            }
        }
    }

    // tig: flip face(=triangle) winding order, so that we are consistent with all other ofPrimitives.
    // i wish there was a more elegant way to do this, but anything happening before "split vertices"
    // makes things very, very complicated.
        
    for (int i = 0; i < indices.size(); i+=3)
        std::swap(indices[i+1], indices[i+2]);

    Mesh sphere;
    sphere.addNormals( &vertices[0][0], vertices.size(), 3 );
    sphere.addTexCoords( &texCoords[0][0], texCoords.size(), 2 );

    for (int i = 0; i < vertices.size(); i++ )
        vertices[i] *= radius;

    sphere.addVertices( &vertices[0][0], vertices.size(), 3 );
    sphere.addIndices( &indices[0], indices.size() );

    return  sphere;
}

Mesh surface(const std::vector<std::vector<glm::vec2>>& _polygon) {

    // Create array
    Mesh mesh;
    BoundingBox bb;

    // using Point = glm::vec2;
    using Coord = float;
    using Point = std::array<Coord, 2>;
    using Line = std::vector<Point>;
    using Polygon = std::vector<Line>;

    std::vector<std::vector<Point>> polygon;
    for (size_t i = 0; i < _polygon.size(); i++) {
        std::vector<Point> polyline;

        for (size_t j = 0; j < _polygon[i].size(); j++ ) {
            mesh.addVertex( _polygon[i][j].x, _polygon[i][j].y, 0.0f );
            bb.expand( _polygon[i][j].x, _polygon[i][j].y );

            polyline.push_back( { _polygon[i][j].x, _polygon[i][j].y} );
        }
        polygon.push_back( polyline );
    }

    for (size_t i = 0; i < mesh.getVerticesTotal(); i++) {
        glm::vec3 p = mesh.getVertices()[i];
        mesh.addTexCoord(   mapValue(p.x, bb.min.x, bb.max.x, 0.0f, 1.0f, true),
                            mapValue(p.y, bb.min.y, bb.max.y, 0.0f, 1.0f, true) );
    }

    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);
    for (size_t i = 0; i < indices.size(); i++)
        mesh.addIndex(indices[i]);    

    return mesh;
}

// Mesh surface(const std::vector<glm::vec2>& _contour) {
//     const std::vector<std::vector<glm::vec2>> polygon;
//     polygon.push_back(_contour);
//     return surface(polygon);
// }

}