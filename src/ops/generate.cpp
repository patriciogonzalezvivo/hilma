#include "hilma/ops/generate.h"

#include "hilma/math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/norm.hpp"

namespace hilma {

Mesh plane(float _width, float _height, int _columns, int _rows, PrimitiveMode _mode ) {
    Mesh mesh;

    if (_mode != TRIANGLE_STRIP && _mode != TRIANGLES)
        _mode = TRIANGLES;

    mesh.setMode(_mode, false);

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

        if (_rows%2 != 0) mesh.addIndex(mesh.getVerticesTotal() - _columns);
    }
    else {

        // Triangles //
        for (int y = 0; y < _rows - 1; y++) {
            for (int x = 0; x < _columns - 1; x++) {
                // first triangle //
                mesh.addIndex((y) *_columns + x);
                mesh.addIndex((y) *_columns + x + 1);
                mesh.addIndex((y + 1) *_columns + x);

                // second triangle //
                mesh.addIndex((y) * _columns + x+1);
                mesh.addIndex((y + 1) *_columns + x+1);
                mesh.addIndex((y + 1) *_columns + x);
            }
        }
    }

    return mesh;
}

Mesh box( float _width, float _height, float _depth, int _resX, int _resY, int _resZ ) {

    // mesh only available as triangles //
    Mesh mesh;
    mesh.setMode( TRIANGLES );

    _resX = _resX + 1;
    _resY = _resY + 1;
    _resZ = _resZ + 1;

    if ( _resX < 2 ) _resX = 0;
    if ( _resY < 2 ) _resY = 0;
    if ( _resZ < 2 ) _resZ = 0;

    // halves //
    float halfW = _width * .5f;
    float halfH = _height * .5f;
    float halfD = _depth * .5f;

    glm::vec3 vert;
    glm::vec3 normal;
    glm::vec2 texcoord;
    std::size_t vertOffset = 0;

    // TRIANGLES //

    // Front Face //
    normal = {0.f, 0.f, 1.f};
    // add the vertexes //
    for (int iy = 0; iy < _resY; iy++) {
        for (int ix = 0; ix < _resX; ix++) {

            // normalized tex coords //
            texcoord.x = ((float)ix/((float)_resX-1.f));
            texcoord.y = 1.f - ((float)iy/((float)_resY-1.f));

            vert.x = texcoord.x * _width - halfW;
            vert.y = -(texcoord.y-1.f) * _height - halfH;
            vert.z = halfD;

            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }

    for (int y = 0; y < _resY-1; y++) {
        for (int x = 0; x < _resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*_resX + x + vertOffset);
            mesh.addIndex((y)*_resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*_resX + x + vertOffset);

            // second triangle //
            mesh.addIndex((y)*_resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*_resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*_resX + x + vertOffset);
        }
    }

    vertOffset = mesh.getVerticesTotal();

    // Right Side Face //
    normal = {1.f, 0.f, 0.f};
    // add the vertexes //
    for (int iy = 0; iy < _resY; iy++) {
        for (int ix = 0; ix < _resZ; ix++) {

            // normalized tex coords //
            texcoord.x = ((float)ix/((float)_resZ-1.f));
            texcoord.y = 1.f - ((float)iy/((float)_resY-1.f));

            //vert.x = texcoord.x * _width - halfW;
            vert.x = halfW;
            vert.y = -(texcoord.y-1.f) * _height - halfH;
            vert.z = texcoord.x * -_depth + halfD;

            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }

    for (int y = 0; y < _resY-1; y++) {
        for (int x = 0; x < _resZ-1; x++) {
            // first triangle //
            mesh.addIndex((y)*_resZ + x + vertOffset);
            mesh.addIndex((y)*_resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*_resZ + x + vertOffset);

            // second triangle //
            mesh.addIndex((y)*_resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*_resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*_resZ + x + vertOffset);
        }
    }

    vertOffset = mesh.getVerticesTotal();

    // Left Side Face //
    normal = {-1.f, 0.f, 0.f};
    // add the vertexes //
    for (int iy = 0; iy < _resY; iy++) {
        for (int ix = 0; ix < _resZ; ix++) {

            // normalized tex coords //
            texcoord.x = ((float)ix/((float)_resZ-1.f));
            texcoord.y = 1.f-((float)iy/((float)_resY-1.f));

            //vert.x = texcoord.x * _width - halfW;
            vert.x = -halfW;
            vert.y = -(texcoord.y-1.f) * _height - halfH;
            vert.z = texcoord.x * _depth - halfD;

            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }

    for (int y = 0; y < _resY-1; y++) {
        for (int x = 0; x < _resZ-1; x++) {
            // first triangle //
            mesh.addIndex((y)*_resZ + x + vertOffset);
            mesh.addIndex((y)*_resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*_resZ + x + vertOffset);

            // second triangle //
            mesh.addIndex((y)*_resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*_resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*_resZ + x + vertOffset);
        }
    }

    vertOffset = mesh.getVerticesTotal();

    // Back Face //
    normal = {0.f, 0.f, -1.f};
    // add the vertexes //
    for (int iy = 0; iy < _resY; iy++) {
        for (int ix = 0; ix < _resX; ix++) {

            // normalized tex coords //
            texcoord.x = ((float)ix/((float)_resX-1.f));
            texcoord.y = 1.f-((float)iy/((float)_resY-1.f));

            vert.x = texcoord.x * -_width + halfW;
            vert.y = -(texcoord.y-1.f) * _height - halfH;
            vert.z = -halfD;

            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }

    for (int y = 0; y < _resY-1; y++) {
        for (int x = 0; x < _resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*_resX + x + vertOffset);
            mesh.addIndex((y)*_resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*_resX + x + vertOffset);

            // second triangle //
            mesh.addIndex((y)*_resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*_resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*_resX + x + vertOffset);
        }
    }

    vertOffset = mesh.getVerticesTotal();


    // Top Face //
    normal = {0.f, -1.f, 0.f};
    // add the vertexes //
    for (int iy = 0; iy < _resZ; iy++) {
        for (int ix = 0; ix < _resX; ix++) {

            // normalized tex coords //
            texcoord.x = ((float)ix/((float)_resX-1.f));
            texcoord.y = 1.f-((float)iy/((float)_resZ-1.f));

            vert.x = texcoord.x * _width - halfW;
            //vert.y = -(texcoord.y-1.f) * _height - halfH;
            vert.y = -halfH;
            vert.z = texcoord.y * _depth - halfD;

            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }

    for (int y = 0; y < _resZ-1; y++) {
        for (int x = 0; x < _resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*_resX + x + vertOffset);
            mesh.addIndex((y+1)*_resX + x + vertOffset);
            mesh.addIndex((y)*_resX + x+1 + vertOffset);

            // second triangle //
            mesh.addIndex((y)*_resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*_resX + x + vertOffset);
            mesh.addIndex((y+1)*_resX + x+1 + vertOffset);
        }
    }

    vertOffset = mesh.getVerticesTotal();

    // Bottom Face //
    normal = {0.f, 1.f, 0.f};
    // add the vertexes //
    for (int iy = 0; iy < _resZ; iy++) {
        for (int ix = 0; ix < _resX; ix++) {

            // normalized tex coords //
            texcoord.x = ((float)ix/((float)_resX-1.f));
            texcoord.y = 1.f-((float)iy/((float)_resZ-1.f));

            vert.x = texcoord.x * _width - halfW;
            //vert.y = -(texcoord.y-1.f) * _height - halfH;
            vert.y = halfH;
            vert.z = texcoord.y * -_depth + halfD;

            mesh.addVertex(vert);
            mesh.addTexCoord(texcoord);
            mesh.addNormal(normal);
        }
    }

    for (int y = 0; y < _resZ-1; y++) {
        for (int x = 0; x < _resX-1; x++) {
            // first triangle //
            mesh.addIndex((y)*_resX + x + vertOffset);
            mesh.addIndex((y+1)*_resX + x + vertOffset);
            mesh.addIndex((y)*_resX + x+1 + vertOffset);

            // second triangle //
            mesh.addIndex((y)*_resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*_resX + x + vertOffset);
            mesh.addIndex((y+1)*_resX + x+1 + vertOffset);
        }
    }

    return mesh;
}

Mesh cube( float _size, int _resolution) {
    return box(_size, _size, _size, _resolution, _resolution, _resolution);
}

Mesh sphere( float _radius, int _resolution, PrimitiveMode _mode ) {
    Mesh mesh;

    float doubleRes = _resolution*2.f;
    float polarInc = PI/(_resolution); // ringAngle
    float azimInc = TAU/(doubleRes); // segAngle //

    if (_mode != TRIANGLE_STRIP && _mode != TRIANGLES)
        _mode = TRIANGLE_STRIP;
    mesh.setMode(_mode);

    glm::vec3 vert;
    glm::vec2 tcoord;

    for (float i = 0; i < _resolution + 1; i++) {

        float tr = sin( PI-i * polarInc );
        float ny = cos( PI-i * polarInc );

        tcoord.y = 1.f - (i / _resolution);

        for (float j = 0; j <= doubleRes; j++) {

            float nx = tr * sin(j * azimInc);
            float nz = tr * cos(j * azimInc);

            tcoord.x = j / (doubleRes);

            vert = {nx, ny, nz};
            mesh.addNormal(vert);

            vert *= _radius;
            mesh.addVertex(vert);
            mesh.addTexCoord(tcoord);
        }
    }

    int nr = doubleRes+1;
    if (_mode == TRIANGLES) {

        int index1, index2, index3;

        for (float iy = 0; iy < _resolution; iy++) {
            for (float ix = 0; ix < doubleRes; ix++) {

                // first tri //
                if (iy > 0) {
                    index1 = (iy+0) * (nr) + (ix+0);
                    index2 = (iy+0) * (nr) + (ix+1);
                    index3 = (iy+1) * (nr) + (ix+0);

                    mesh.addIndex(index1);
                    mesh.addIndex(index2);
                    mesh.addIndex(index3);
                }

                if (iy < _resolution - 1 ) {
                    // second tri //
                    index1 = (iy+0) * (nr) + (ix+1);
                    index2 = (iy+1) * (nr) + (ix+1);
                    index3 = (iy+1) * (nr) + (ix+0);

                    mesh.addIndex(index1);
                    mesh.addIndex(index2);
                    mesh.addIndex(index3);

                }
            }
        }

    }
    else {
        for (int y = 0; y < _resolution; y++) {
            for (int x = 0; x <= doubleRes; x++) {
                mesh.addIndex( (y)*nr + x );
                mesh.addIndex( (y+1)*nr + x );
            }
        }
    }


    return mesh;
}

Mesh icosphere(float _radius, size_t _iterations) {

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
       
    std::vector<INDEX_TYPE> indices = {
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

    size_t size = indices.size();

    /// Step 2 : tessellate
    for (size_t iteration = 0; iteration < _iterations; iteration++) {
        size*=4;
        std::vector<INDEX_TYPE> newFaces;
        for (size_t i = 0; i < size/12; i++) {
            INDEX_TYPE i1 = indices[i*3];
            INDEX_TYPE i2 = indices[i*3+1];
            INDEX_TYPE i3 = indices[i*3+2];
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
    for (size_t i = 0; i < vertices.size(); i++) {
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

    for (size_t i=0; i<indices.size()/3; i++) {
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
    for (size_t i = 0; i < indexToSplit.size(); i++) {
        INDEX_TYPE index = indexToSplit[i];
        //duplicate vertex
        glm::vec3 v = vertices[index];
        glm::vec2 t = texCoords[index] + glm::vec2(1.f, 0.f);
        vertices.push_back(v);
        texCoords.push_back(t);
        size_t newIndex = vertices.size()-1;
        //reassign indices
        for (size_t j = 0; j<indices.size(); j++) {
            if (indices[j] == index) {
                INDEX_TYPE index1 = indices[(j+1)%3+(j/3)*3];
                INDEX_TYPE index2 = indices[(j+2)%3+(j/3)*3];
                if ((texCoords[index1].x>0.5) || (texCoords[index2].x>0.5))
                    indices[j] = newIndex;
            }
        }
    }

    Mesh mesh;
    mesh.addNormals( &vertices[0][0], vertices.size(), 3 );
    mesh.addTexCoords( &texCoords[0][0], texCoords.size(), 2 );

    for (size_t i = 0; i < vertices.size(); i++ )
        vertices[i] *= _radius;

    mesh.addVertices( &vertices[0][0], vertices.size(), 3 );
    mesh.addIndices( &indices[0], indices.size() );

    return  mesh;
}

Mesh cylinder( float _radius, float _height, int _radiusSegments, int _heightSegments, int _numCapSegments, bool _bCapped, PrimitiveMode _mode ) {
    Mesh mesh;
    if (_mode != TRIANGLE_STRIP && _mode != TRIANGLES)
        _mode = TRIANGLE_STRIP;
        
    mesh.setMode(_mode, false);

    _radiusSegments = _radiusSegments+1;
    int capSegs = _numCapSegments;
    capSegs = capSegs+1;
    _heightSegments = _heightSegments+1;
    if (_heightSegments < 2) _heightSegments = 2;
    if ( capSegs < 2 ) _bCapped = false;
    if (!_bCapped) capSegs=1;

    float angleIncRadius = -1 * (TAU/((float)_radiusSegments-1.f));
    float heightInc = _height/((float)_heightSegments-1.f);
    float halfH = _height*.5f;

    float newRad;
    glm::vec3 vert;
    glm::vec2 tcoord;
    glm::vec3 normal;
    glm::vec3 up(0,1,0);

    std::size_t vertOffset = 0;

    float maxTexY   = _heightSegments-1.f;
    if (capSegs > 0)
        maxTexY += (capSegs*2)-2.f;
        
    float maxTexYNormalized = (capSegs-1.f) / maxTexY;

    // add the top cap //
    if (_bCapped && capSegs > 0) {
        normal = {0.f, -1.f, 0.f};
        for (int iy = 0; iy < capSegs; iy++) {
            for (int ix = 0; ix < _radiusSegments; ix++) {
                newRad = mapValue((float)iy, 0, capSegs-1, 0.0, _radius, true);
                vert.x = cos((float)ix*angleIncRadius) * newRad;
                vert.z = sin((float)ix*angleIncRadius) * newRad;
                vert.y = -halfH;

                tcoord.x = (float)ix/((float)_radiusSegments-1.f);
                tcoord.y = 1.f - mapValue(iy, 0, capSegs-1, 0, maxTexYNormalized, true);

                mesh.addTexCoord( tcoord );
                mesh.addVertex( vert );
                mesh.addNormal( normal );
            }
        }

        if (_mode == TRIANGLES) {
            for (int y = 0; y < capSegs-1; y++) {
                for (int x = 0; x < _radiusSegments-1; x++) {
                    if (y > 0) {
                        // first triangle //
                        mesh.addIndex( (y)*_radiusSegments + x + vertOffset );
                        mesh.addIndex( (y)*_radiusSegments + x+1 + vertOffset);
                        mesh.addIndex( (y+1)*_radiusSegments + x + vertOffset);
                    }

                    // second triangle //
                    mesh.addIndex( (y)*_radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*_radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*_radiusSegments + x + vertOffset);
                }
            }
        } else {
            for (int y = 0; y < capSegs-1; y++) {
                for (int x = 0; x < _radiusSegments; x++) {
                    mesh.addIndex( (y)*_radiusSegments + x + vertOffset );
                    mesh.addIndex( (y+1)*_radiusSegments + x + vertOffset);
                }
            }
        }

        vertOffset = mesh.getVerticesTotal();

    }

    //maxTexY            = _heightSegments-1.f + capSegs-1.f;
    float minTexYNormalized = 0;
    if (_bCapped) minTexYNormalized = maxTexYNormalized;
    maxTexYNormalized   = 1.f;
    if (_bCapped) maxTexYNormalized = (_heightSegments) / maxTexY;

    // cylinder vertices //
    for (int iy = 0; iy < _heightSegments; iy++) {
        normal = {1.f, 0.f, 0.f};
        for (int ix = 0; ix < _radiusSegments; ix++) {

            //newRad = mapValue((float)iy, 0, _heightSegments-1, 0.0, radius);
            vert.x = cos(ix*angleIncRadius) * _radius;
            vert.y = heightInc*float(iy) - halfH;
            vert.z = sin(ix*angleIncRadius) * _radius;

            tcoord.x = float(ix)/(float(_radiusSegments)-1.f);
            tcoord.y = 1.f - mapValue(iy, 0, _heightSegments-1, minTexYNormalized, maxTexYNormalized, true );

            mesh.addTexCoord( tcoord );
            mesh.addVertex( vert );
            mesh.addNormal( normal );

            normal = glm::rotate(normal, -angleIncRadius, up);

        }
    }

    if (_mode == TRIANGLES) {
        for (int y = 0; y < _heightSegments-1; y++) {
            for (int x = 0; x < _radiusSegments-1; x++) {
                // first triangle //
                mesh.addIndex( (y)*_radiusSegments + x + vertOffset);
                mesh.addIndex( (y)*_radiusSegments + x+1 + vertOffset );
                mesh.addIndex( (y+1)*_radiusSegments + x + vertOffset );

                // second triangle //
                mesh.addIndex( (y)*_radiusSegments + x+1 + vertOffset );
                mesh.addIndex( (y+1)*_radiusSegments + x+1 + vertOffset );
                mesh.addIndex( (y+1)*_radiusSegments + x + vertOffset );
            }
        }
    } else {
        for (int y = 0; y < _heightSegments-1; y++) {
            for (int x = 0; x < _radiusSegments; x++) {
                mesh.addIndex( (y)*_radiusSegments + x + vertOffset );
                mesh.addIndex( (y+1)*_radiusSegments + x + vertOffset );
            }
        }
    }

    vertOffset = mesh.getVerticesTotal();

    // add the bottom cap
    if (_bCapped && capSegs > 0) {
        minTexYNormalized = maxTexYNormalized;
        maxTexYNormalized   = 1.f;

        normal = {0.f, 1.f, 0.f};
        for (int iy = 0; iy < capSegs; iy++) {
            for (int ix = 0; ix < _radiusSegments; ix++) {
                newRad = mapValue((float)iy, 0, capSegs-1, _radius, 0.0, true);
                vert.x = cos((float)ix*angleIncRadius) * newRad;
                vert.z = sin((float)ix*angleIncRadius) * newRad;
                vert.y = halfH;

                tcoord.x = (float)ix/((float)_radiusSegments-1.f);
                tcoord.y = 1.f - mapValue(iy, 0, capSegs-1, minTexYNormalized, maxTexYNormalized, true);

                mesh.addTexCoord( tcoord );
                mesh.addVertex( vert );
                mesh.addNormal( normal );
            }
        }

        if (_mode == TRIANGLES) {
            for (int y = 0; y < capSegs-1; y++) {
                for (int x = 0; x < _radiusSegments-1; x++) {
                    // first triangle //
                    mesh.addIndex( (y)*_radiusSegments + x + vertOffset );
                    mesh.addIndex( (y)*_radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*_radiusSegments + x + vertOffset);

                    if (y < capSegs -1 && capSegs > 2) {
                        // second triangle //
                        mesh.addIndex( (y)*_radiusSegments + x+1 + vertOffset);
                        mesh.addIndex( (y+1)*_radiusSegments + x+1 + vertOffset);
                        mesh.addIndex( (y+1)*_radiusSegments + x + vertOffset);
                    }
                }
            }
        } else {
            for (int y = 0; y < capSegs-1; y++) {
                for (int x = 0; x < _radiusSegments; x++) {
                    mesh.addIndex( (y)*_radiusSegments + x + vertOffset );
                    mesh.addIndex( (y+1)*_radiusSegments + x + vertOffset);
                }
            }
        }

        vertOffset = mesh.getVerticesTotal();

    }

    return mesh;
}

Mesh cone( float radius, float _height, int _radiusSegments, int _heightSegments, int _capSegments, PrimitiveMode _mode ) {
    Mesh mesh;
    if (_mode != TRIANGLE_STRIP && _mode != TRIANGLES)
        _mode = TRIANGLE_STRIP;

    mesh.setMode(_mode);

    _radiusSegments = _radiusSegments+1;
    _capSegments = _capSegments+1;
    _heightSegments = _heightSegments+1;
    if (_heightSegments < 2) _heightSegments = 2;
    int capSegs = _capSegments;
    if ( capSegs < 2 )
        capSegs = 0;

    float angleIncRadius = -1.f * ((TAU/((float)_radiusSegments-1.f)));
    float heightInc = _height/((float)_heightSegments-1);
    float halfH = _height*.5f;

    float newRad;
    glm::vec3 vert;
    glm::vec3 normal;
    glm::vec2 tcoord;
    glm::vec3 up(0.0,1.0,0.0);

    std::size_t vertOffset = 0;

    float maxTexY = _heightSegments-1.f;
    if (capSegs > 0) 
        maxTexY += capSegs-1.f;

    glm::vec3 startVec(0, -halfH-1.f, 0);

    // cone vertices //
    for (int iy = 0; iy < _heightSegments; iy++) {
        for (int ix = 0; ix < _radiusSegments; ix++) {

            newRad = mapValue((float)iy, 0, _heightSegments-1, 0.0, radius, true);
            vert.x = cos((float)ix*angleIncRadius) * newRad;
            vert.y = heightInc*((float)iy) - halfH;
            vert.z = sin((float)ix*angleIncRadius) * newRad;

            tcoord.x = (float)ix/((float)_radiusSegments-1.f);
            tcoord.y = 1.f - (float)iy/((float)maxTexY);

            mesh.addTexCoord( tcoord );
            mesh.addVertex( vert );

            if (iy == 0) {
                newRad = 1.f;
                vert.x = cos((float)ix*angleIncRadius) * newRad;
                vert.y = heightInc*((float)iy) - halfH;
                vert.z = sin((float)ix*angleIncRadius) * newRad;
            }

            glm::vec3 diff = vert - startVec;
            glm::vec3 crossed = glm::cross(up, vert);
            normal = glm::cross(crossed, diff);
            mesh.addNormal( glm::normalize(normal) );

        }
    }

    if (_mode == TRIANGLES) {
        for (int y = 0; y < _heightSegments-1; y++) {
            for (int x = 0; x < _radiusSegments-1; x++) {
                if (y > 0){
                    // first triangle //
                    mesh.addIndex( (y)*_radiusSegments + x );
                    mesh.addIndex( (y)*_radiusSegments + x+1 );
                    mesh.addIndex( (y+1)*_radiusSegments + x );
                }

                // second triangle //
                mesh.addIndex( (y)*_radiusSegments + x+1 );
                mesh.addIndex( (y+1)*_radiusSegments + x+1 );
                mesh.addIndex( (y+1)*_radiusSegments + x );
            }
        }
    } else {
        for (int y = 0; y < _heightSegments-1; y++) {
            for (int x = 0; x < _radiusSegments; x++) {
                mesh.addIndex( (y)*_radiusSegments + x );
                mesh.addIndex( (y+1)*_radiusSegments + x );
            }
        }
    }

    vertOffset = mesh.getVerticesTotal();
    float maxTexYNormalized = (_heightSegments-1.f) / maxTexY;

    // add the cap //
    normal= {0.f,1.f,0.f};
    for (int iy = 0; iy < capSegs; iy++) {
        for (int ix = 0; ix < _radiusSegments; ix++) {
            newRad = mapValue((float)iy, 0, capSegs-1, radius, 0.0, true);
            vert.x = cos((float)ix*angleIncRadius) * newRad;
            vert.z = sin((float)ix*angleIncRadius) * newRad;
            vert.y = halfH;

            tcoord.x = (float)ix/((float)_radiusSegments-1.f);
            tcoord.y = 1.f - mapValue(iy, 0, capSegs-1, maxTexYNormalized, 1.f, true);

            mesh.addTexCoord( tcoord );
            mesh.addVertex( vert );
            mesh.addNormal( normal );
        }
    }

    if (_mode == TRIANGLES) {
        if ( capSegs > 0 ) {
            for (int y = 0; y < capSegs-1; y++) {
                for (int x = 0; x < _radiusSegments-1; x++) {
                    // first triangle //
                    mesh.addIndex( (y)*_radiusSegments + x + vertOffset );
                    mesh.addIndex( (y)*_radiusSegments + x+1 + vertOffset);
                    mesh.addIndex( (y+1)*_radiusSegments + x + vertOffset);

                    if (y < capSegs-1) {
                        // second triangle //
                        mesh.addIndex( (y)*_radiusSegments + x+1 + vertOffset);
                        mesh.addIndex( (y+1)*_radiusSegments + x+1 + vertOffset);
                        mesh.addIndex( (y+1)*_radiusSegments + x + vertOffset);
                    }
                }
            }
        }
    } else {
        if (capSegs > 0 ) {
            for (int y = 0; y < capSegs-1; y++) {
                for (int x = 0; x < _radiusSegments; x++) {
                    mesh.addIndex( (y)*_radiusSegments + x + vertOffset );
                    mesh.addIndex( (y+1)*_radiusSegments + x + vertOffset);
                }
            }
        }
    }

    return mesh;
}

}