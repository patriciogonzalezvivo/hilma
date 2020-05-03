#include "hilma/ops/generate.h"
#include "hilma/ops/earcut.h"

#include "hilma/math.h"
#include "hilma/types/BoundingBox.h"

 #define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/norm.hpp"

// namespace mapbox { namespace util {
// template <>
// struct nth<0, glm::vec2> {
//     inline static float get(const glm::vec2 &t) { return t.x; };
// };
// template <>
// struct nth<1, glm::vec2> {
//     inline static float get(const glm::vec2 &t) { return t.y; };
// };
// }}

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
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);

            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
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
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);

            // second triangle //
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
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
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);

            // second triangle //
            mesh.addIndex((y)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x+1 + vertOffset);
            mesh.addIndex((y+1)*resZ + x + vertOffset);
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
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);

            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
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
            mesh.addIndex((y+1)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);

            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
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
            mesh.addIndex((y+1)*resX + x + vertOffset);
            mesh.addIndex((y)*resX + x+1 + vertOffset);

            // second triangle //
            mesh.addIndex((y)*resX + x+1 + vertOffset);
            mesh.addIndex((y+1)*resX + x + vertOffset);
            mesh.addIndex((y+1)*resX + x+1 + vertOffset);
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
                    mesh.addIndex(index2);
                    mesh.addIndex(index3);
                }

                if(iy < res-1 ) {
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

    Mesh mesh;
    mesh.addNormals( &vertices[0][0], vertices.size(), 3 );
    mesh.addTexCoords( &texCoords[0][0], texCoords.size(), 2 );

    for (int i = 0; i < vertices.size(); i++ )
        vertices[i] *= radius;

    mesh.addVertices( &vertices[0][0], vertices.size(), 3 );
    mesh.addIndices( &indices[0], indices.size() );

    return  mesh;
}

Mesh surface(const std::vector<std::vector<glm::vec2>>& _polygon) {

    // Create array
    Mesh mesh;
    static const glm::vec3 upVector(0.0f, 0.0f, 1.0f);

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
            mesh.addNormal( upVector );
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

Mesh surface(const std::vector<glm::vec2>& _contour) {
    std::vector<std::vector<glm::vec2>> polygon;
    polygon.push_back(_contour);
    return surface(polygon);
}

Mesh extrudeWall(const std::vector<glm::vec2>& _contour, float _maxHeight, float _minHeight) {
    std::vector<std::vector<glm::vec2>> polygon;
    polygon.push_back(_contour);
    return extrudeWall(polygon, _maxHeight, _minHeight);
}

Mesh extrudeWall(const std::vector<std::vector<glm::vec2>>& _polygon, float _maxHeight, float _minHeight) {
    Mesh mesh;
    
    static const glm::vec3 upVector(0.0f, 0.0f, 1.0f);
    glm::vec3 normalVector;

    int lineN  = 0;
    int vertexN = 0;
    for (auto& line : _polygon) {
        size_t lineSize = line.size();

        for (size_t i = 0; i < lineSize - 1; i++) {

            glm::vec3 a(line[i], 0.f);
            glm::vec3 b(line[i+1], 0.f);

            
            normalVector = glm::cross(upVector, b - a);
            normalVector = glm::normalize(normalVector);

            if (std::isnan(normalVector.x)
             || std::isnan(normalVector.y)
             || std::isnan(normalVector.z)) {
                continue;
            }

            // 1st vertex top
            a.z = _maxHeight;
            mesh.addVertex(a);
            mesh.addNormal(normalVector);
            mesh.addTexCoord(1.,1.);

            // 2nd vertex top
            b.z = _maxHeight;
            mesh.addVertex(b);
            mesh.addNormal(normalVector);
            mesh.addTexCoord(0.,1.);

            // 1st vertex bottom
            a.z = _minHeight;
            mesh.addVertex(a);
            mesh.addNormal(normalVector);
            mesh.addTexCoord(1.,0.);

            // 2nd vertex bottom
            b.z = _minHeight;
            mesh.addVertex(b);
            mesh.addNormal(normalVector);
            mesh.addTexCoord(0.,0.);

            // Start the index from the previous state of the vertex Data
            if (lineN == 0) {
                mesh.addTriangle(vertexN, vertexN + 2, vertexN + 1);
                mesh.addTriangle(vertexN + 1, vertexN + 2, vertexN + 3);
            }
            else {
                mesh.addTriangle(vertexN, vertexN + 1, vertexN + 2);
                mesh.addTriangle(vertexN + 1, vertexN + 3, vertexN + 2);
            }

            vertexN += 4;
        }

        lineN ++;
    }

    return mesh;
}

// From Tangram
// https://github.com/tangrams/tangram-es/blob/e4a323afeb310520456aec49e338614120a7ffa2/core/src/util/builders.cpp

// Get 2D perpendicular of two points
glm::vec2 perp2d(const glm::vec2& _v1, const glm::vec2& _v2 ){
    return glm::vec2(_v2.y - _v1.y, _v1.x - _v2.x);
}

// Helper function for polyline tesselation
inline void addPolyLineVertex(const glm::vec2& _coord, const glm::vec2& _normal, const glm::vec2& _uv, Mesh& _mesh, float _width) {
    if (_width > 0.0f) {
        glm::vec2 p = _coord +_normal * _width;
        _mesh.addVertex(p.x, p.y);
        _mesh.addNormal(0.0f, 0.0f, 1.0f);
    }
    else {
        // Collapsed spline 
        _mesh.addVertex(_coord.x, _coord.y);
        _mesh.addNormal(_normal.x, _normal.y, 0.0f);
    }
    _mesh.addTexCoord(_uv);
}

// Helper function for polyline tesselation; adds indices for pairs of vertices arranged like a line strip
void indexPairs( int _nPairs, Mesh& _mesh) {
    int nVertices = _mesh.getVerticesTotal();

    for (int i = 0; i < _nPairs; i++) {
        _mesh.addTriangle(  nVertices - 2*i - 4,
                            nVertices - 2*i - 2,
                            nVertices - 2*i - 3 );

        _mesh.addTriangle(  nVertices - 2*i - 3,
                            nVertices - 2*i - 2,
                            nVertices - 2*i - 1 );
    }
}

//  Tessalate a fan geometry between points A       B
//  using their normals from a center        \ . . /
//  and interpolating their UVs               \ p /
//                                             \./
//                                              C
void addFan(const glm::vec2& _pC,
            const glm::vec2& _nA, const glm::vec2& _nB, const glm::vec2& _nC,
            const glm::vec2& _uA, const glm::vec2& _uB, const glm::vec2& _uC,
            int _numTriangles, Mesh& _mesh, float _width) {

    // Find angle difference
    float cross = _nA.x * _nB.y - _nA.y * _nB.x; // z component of cross(_CA, _CB)
    float angle = atan2f(cross, glm::dot(_nA, _nB));

    int startIndex = _mesh.getVerticesTotal();

    // Add center vertex
    addPolyLineVertex(_pC, _nC, _uC, _mesh, _width);

    // Add vertex for point A
    addPolyLineVertex(_pC, _nA, _uA, _mesh, _width);

    // Add radial vertices
    glm::vec2 radial = _nA;
    for (int i = 0; i < _numTriangles; i++) {
        float frac = (i + 1)/(float)_numTriangles;
        radial = glm::rotate(_nA, angle * frac);

        glm::vec2 uv(0.0);
        // if (_mesh.useTexCoords)
        uv = (1.f - frac) * _uA + frac * _uB;

        addPolyLineVertex(_pC, radial, uv, _mesh, _width);

        // Add indices
        _mesh.addTriangle(  startIndex, // center vertex
                            startIndex + i + (angle > 0 ? 1 : 2),
                            startIndex + i + (angle > 0 ? 2 : 1) );
    }

}

// Function to add the vertices for line caps
void addCap(const glm::vec2& _coord, const glm::vec2& _normal, int _numCorners, bool _isBeginning, Mesh& _mesh, float _width) {

    float v = _isBeginning ? 0.f : 1.f; // length-wise tex coord

    if (_numCorners < 1) {
        // "Butt" cap needs no extra vertices
        return;
    } else if (_numCorners == 2) {
        // "Square" cap needs two extra vertices
        glm::vec2 tangent(-_normal.y, _normal.x);
        addPolyLineVertex(_coord, _normal + tangent, {0.f, v}, _mesh, _width);
        addPolyLineVertex(_coord, -_normal + tangent, {0.f, v}, _mesh, _width);
        if (!_isBeginning) { // At the beginning of a line we can't form triangles with previous vertices
            indexPairs(1, _mesh);
        }
        return;
    }

    // "Round" cap type needs a fan of vertices
    glm::vec2 nA(_normal), nB(-_normal), nC(0.f, 0.f), uA(1.f, v), uB(0.f, v), uC(0.5f, v);
    if (_isBeginning) {
        nA *= -1.f; // To flip the direction of the fan, we negate the normal vectors
        nB *= -1.f;
        uA.x = 0.f; // To keep tex coords consistent, we must reverse these too
        uB.x = 1.f;
    }
    addFan(_coord, nA, nB, nC, uA, uB, uC, _numCorners, _mesh, _width);
}

void buildPolyLineSegment(  const std::vector<glm::vec2>& _line, Mesh& _mesh, size_t _startIndex, size_t _endIndex,  
                            float _width, JoinType _join, CapType _cap, bool endCap, float _miterLimit) {

    float distance = 0; // Cumulative distance along the polyline.

    size_t origLineSize = _line.size();

    // endIndex/startIndex could be wrapped values, calculate lineSize accordingly
    int lineSize = (int)((_endIndex > _startIndex) ?
                   (_endIndex - _startIndex) :
                   (origLineSize - _startIndex + _endIndex));
    if (lineSize < 2) { return; }

    glm::vec2 coordCurr(_line[_startIndex]);
    // get the Point using wrapped index in the original line geometry
    glm::vec2 coordNext(_line[(_startIndex + 1) % origLineSize]);
    glm::vec2 normPrev, normNext, miterVec;

    int cornersOnCap = (int)_cap;
    int trianglesOnJoin = (int)_join;

    // Process first point in line with an end cap
    normNext = glm::normalize(perp2d(coordCurr, coordNext));

    if (endCap)
        addCap(coordCurr, normNext, cornersOnCap, true, _mesh, _width);
    
    addPolyLineVertex(coordCurr, normNext, {1.0f, 0.0f}, _mesh, _width); // right corner
    addPolyLineVertex(coordCurr, -normNext, {0.0f, 0.0f}, _mesh, _width); // left corner


    // Process intermediate points
    for (int i = 1; i < lineSize - 1; i++) {
        // get the Point using wrapped index in the original line geometry
        int nextIndex = (i + _startIndex + 1) % origLineSize;

        distance += glm::distance(coordCurr, coordNext);

        coordCurr = coordNext;
        coordNext = _line[nextIndex];

        if (coordCurr == coordNext)
            continue;

        normPrev = normNext;
        normNext = glm::normalize(perp2d(coordCurr, coordNext));

        // Compute "normal" for miter joint
        miterVec = normPrev + normNext;

        float scale = 1.f;

        // normPrev and normNext are in the opposite direction
        // in order to prevent NaN values, we use the perp
        // vector of those two vectors
        if (miterVec == glm::zero<glm::vec2>())
            miterVec = perp2d(glm::vec3(normNext, 0.f), glm::vec3(normPrev, 0.f));
        else
            scale = 2.f / glm::dot(miterVec, miterVec);

        miterVec *= scale;

        if (glm::length2(miterVec) > glm::length2(_miterLimit)) {
            trianglesOnJoin = 1;
            miterVec *= _miterLimit / glm::length(miterVec);
        }

        float v = distance;

        if (trianglesOnJoin == 0) {
            // Join type is a simple miter

            addPolyLineVertex(coordCurr, miterVec, {1.0, v}, _mesh, _width); // right corner
            addPolyLineVertex(coordCurr, -miterVec, {0.0, v}, _mesh, _width); // left corner
            indexPairs(1, _mesh);

        }
        else {

            // Join type is a fan of triangles

            bool isRightTurn = (normNext.x * normPrev.y - normNext.y * normPrev.x) > 0; // z component of cross(normNext, normPrev)

            if (isRightTurn) {

                addPolyLineVertex(coordCurr, miterVec, {1.0f, v}, _mesh, _width); // right (inner) corner
                addPolyLineVertex(coordCurr, -normPrev, {0.0f, v}, _mesh, _width); // left (outer) corner
                indexPairs(1, _mesh);

                addFan(coordCurr, -normPrev, -normNext, miterVec, {0.f, v}, {0.f, v}, {1.f, v}, trianglesOnJoin, _mesh, _width);

                addPolyLineVertex(coordCurr, miterVec, {1.0f, v}, _mesh, _width); // right (inner) corner
                addPolyLineVertex(coordCurr, -normNext, {0.0f, v}, _mesh, _width); // left (outer) corner

            } else {

                addPolyLineVertex(coordCurr, normPrev, {1.0f, v}, _mesh, _width); // right (outer) corner
                addPolyLineVertex(coordCurr, -miterVec, {0.0f, v}, _mesh, _width); // left (inner) corner
                indexPairs(1, _mesh);

                addFan(coordCurr, normPrev, normNext, -miterVec, {1.f, v}, {1.f, v}, {0.0f, v}, trianglesOnJoin, _mesh, _width);

                addPolyLineVertex(coordCurr, normNext, {1.0f, v}, _mesh, _width); // right (outer) corner
                addPolyLineVertex(coordCurr, -miterVec, {0.0f, v}, _mesh, _width); // left (inner) corner
            }
        }
    }

    distance += glm::distance(coordCurr, coordNext);

    // Process last point in line with a cap
    addPolyLineVertex(coordNext, normNext, {1.f, distance}, _mesh, _width); // right corner
    addPolyLineVertex(coordNext, -normNext, {0.f, distance}, _mesh, _width); // left corner
    indexPairs(1, _mesh);
    if (endCap)
        addCap(coordNext, normNext, cornersOnCap, false, _mesh, _width);
}

Mesh spline(const std::vector<glm::vec2>& _polyline, float _width, JoinType _join, CapType _cap, float _miterLimit) { //}, bool _close) {

    size_t lineSize = _polyline.size();
    Mesh mesh;

    // if (keepTileEdges) {
        buildPolyLineSegment(_polyline, mesh, 0, lineSize, _width, _join, _cap, true, _miterLimit);

    // } 
    // else {

        // int cut = 0;
        // int firstCutEnd = 0;

        // // Determine cuts
        // for (size_t i = 0; i < lineSize - 1; i++) {
        //     const glm::vec2& coordCurr = _polyline[i];
        //     const glm::vec2& coordNext = _polyline[i+1];
        //     if (isOutsideTile(coordCurr, coordNext)) {
        //         if (cut == 0) {
        //             firstCutEnd = i + 1;
        //         }
        //         buildPolyLineSegment(_polyline, _mesh, cut, i + 1);
        //         cut = i + 1;
        //     }
        // }

        // if (closedPolygon) {
        //     if (cut == 0) {
        //         // no tile edge cuts!
        //         // loop and close the polygon with no endcaps
        //         buildPolyLineSegment(_polyline, _mesh, 0, lineSize+2, false);
        //     }
        //     else {
        //         // merge first and last cut line-segments together
        //         buildPolyLineSegment(_polyline, _mesh, cut, firstCutEnd);
        //     }
        // }
        // else {
        //     buildPolyLineSegment(_polyline, _mesh, cut, lineSize);
        // }
    // }
    return mesh;
}

}