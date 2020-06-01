#include "hilma/ops/convert_path.h"
#include "hilma/ops/earcut.h"

#include "hilma/math.h"
#include "hilma/accel/BoundingBox.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
// #include "glm/gtx/norm.hpp"
// #include "glm/gtx/hash.hpp"

// #include <map>
// #include <unordered_map>

#include <algorithm>

namespace mapbox { namespace util {

template <>
struct nth<0, glm::vec3> {
    inline static float get(const glm::vec3 &t) { return t.x; };
};
template <>
struct nth<1, glm::vec3> {
    inline static float get(const glm::vec3 &t) { return t.y; };
};

}}


namespace hilma {

Mesh toSurface(const std::vector<std::vector<glm::vec3>>& _polygon, BoundingBox& _bbox) {
    Mesh mesh;

    BoundingBox bb;
    static const glm::vec3 upVector(0.0f, 0.0f, 1.0f);
    for (size_t i = 0; i < _polygon.size(); i++) {
        for (size_t j = 0; j < _polygon[i].size(); j++ ) {
            mesh.addVertex( _polygon[i][j].x, _polygon[i][j].y, 0.0f );
            mesh.addNormal( upVector );
            bb.expand( _polygon[i][j].x, _polygon[i][j].y );
        }
    }

    for (size_t i = 0; i < mesh.getVerticesTotal(); i++) {
        glm::vec3 p = mesh.getVertices()[i];
        mesh.addTexCoord(   remap(p.x, bb.min.x, bb.max.x, 0.0f, 1.0f, true),
                            remap(p.y, bb.min.y, bb.max.y, 0.0f, 1.0f, true) );
    }

    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(_polygon);
    for (size_t i = 0; i < indices.size(); i++)
        mesh.addFaceIndex(indices[i]);

    return mesh;
}

Mesh toSurface(const std::vector<std::vector<glm::vec3>>& _polygon, const BoundingBox& _bbox) {
    Mesh mesh;

    BoundingBox bb = _bbox;
    static const glm::vec3 upVector(0.0f, 0.0f, 1.0f);
    for (size_t i = 0; i < _polygon.size(); i++) {
        for (size_t j = 0; j < _polygon[i].size(); j++ ) {
            mesh.addVertex( _polygon[i][j].x, _polygon[i][j].y, 0.0f );
            mesh.addNormal( upVector );
            bb.expand( _polygon[i][j].x, _polygon[i][j].y );
        }
    }

    for (size_t i = 0; i < mesh.getVerticesTotal(); i++) {
        glm::vec3 p = mesh.getVertices()[i];
        mesh.addTexCoord(   remap(p.x, bb.min.x, bb.max.x, 0.0f, 1.0f, true),
                            remap(p.y, bb.min.y, bb.max.y, 0.0f, 1.0f, true) );
    }

    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(_polygon);
    for (size_t i = 0; i < indices.size(); i++)
        mesh.addFaceIndex(indices[i]);

    return mesh;
}

Mesh toWall(const std::vector<glm::vec3>& _polyline, float _maxHeight, float _minHeight) {
    Mesh mesh;
    
    static const glm::vec3 upVector(0.0f, 0.0f, 1.0f);
    glm::vec3 normalVector;

    int lineN  = 0;
    INDEX_TYPE vertexN = 0;
    for (size_t i = 0; i < _polyline.size() - 1; i++) {

        glm::vec3 a(_polyline[i].x, _polyline[i].y, 0.f);
        glm::vec3 b(_polyline[i+1].x, _polyline[i+1].y, 0.f);

        
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
            mesh.addTriangleIndices(vertexN, vertexN + 2, vertexN + 1);
            mesh.addTriangleIndices(vertexN + 1, vertexN + 2, vertexN + 3);
        }
        else {
            mesh.addTriangleIndices(vertexN, vertexN + 1, vertexN + 2);
            mesh.addTriangleIndices(vertexN + 1, vertexN + 3, vertexN + 2);
        }

        vertexN += 4;
    }

    return mesh;
}

// From Tangram
// https://github.com/tangrams/tangram-es/blob/e4a323afeb310520456aec49e338614120a7ffa2/core/src/util/Modifys.cpp

// Get 2D perpendicular of two points

inline glm::vec2 perp2d(const glm::vec3& _v1, const glm::vec3& _v2 ) {
    return glm::vec2(_v2.y - _v1.y, _v1.x - _v2.x);
}

// Helper function for polyline tesselation
inline void addPolyLineVertex(const glm::vec3& _coord, const glm::vec2& _normal, const glm::vec2& _uv, Mesh& _mesh, float _width) {
    if (_width > 0.0f) {
        glm::vec3 p = _coord + glm::vec3(_normal, 0.0f) * _width;
        _mesh.addVertex(p);
        _mesh.addNormal(0.0f, 0.0f, 1.0f);
    }
    else {
        // Collapsed spline 
        _mesh.addVertex(_coord);
        _mesh.addNormal(_normal.x, _normal.y, 0.0f);
    }
    _mesh.addTexCoord(_uv);
}

// Helper function for polyline tesselation; adds indices for pairs of vertices arranged like a line strip
void indexPairs(size_t _nPairs, Mesh& _mesh) {
    size_t nVertices = _mesh.getVerticesTotal();

    for (size_t i = 0; i < _nPairs; i++) {
        _mesh.addTriangleIndices(   nVertices - 2*i - 4,
                                    nVertices - 2*i - 2,
                                    nVertices - 2*i - 3 );

        _mesh.addTriangleIndices(   nVertices - 2*i - 3,
                                    nVertices - 2*i - 2,
                                    nVertices - 2*i - 1 );
    }
}

//  Tessalate a fan geometry between points A       B
//  using their normals from a center        \ . . /
//  and interpolating their UVs               \ p /
//                                             \./
//                                              C
void addFan(const glm::vec3& _pC,
            const glm::vec2& _nA, const glm::vec2& _nB, const glm::vec2& _nC,
            const glm::vec2& _uA, const glm::vec2& _uB, const glm::vec2& _uC,
            size_t _numTriangles, Mesh& _mesh, float _width) {

    // Find angle difference
    float cross = _nA.x * _nB.y - _nA.y * _nB.x; // z component of cross(_CA, _CB)
    float angle = atan2f(cross, glm::dot(_nA, _nB));

    size_t startIndex = _mesh.getVerticesTotal();

    // Add center vertex
    addPolyLineVertex(_pC, _nC, _uC, _mesh, _width);

    // Add vertex for point A
    addPolyLineVertex(_pC, _nA, _uA, _mesh, _width);

    // Add radial vertices
    glm::vec2 radial = _nA;
    for (size_t i = 0; i < _numTriangles; i++) {
        float frac = (i + 1)/(float)_numTriangles;
        radial = glm::rotate(_nA, angle * frac);

        glm::vec2 uv(0.0);
        // if (_mesh.useTexCoords)
        uv = (1.f - frac) * _uA + frac * _uB;

        addPolyLineVertex(_pC, radial, uv, _mesh, _width);

        // Add indices
        _mesh.addTriangleIndices(   startIndex, // center vertex
                                    startIndex + i + (angle > 0 ? 1 : 2),
                                    startIndex + i + (angle > 0 ? 2 : 1) );
    }

}

// Function to add the vertices for line caps
void addCap(const glm::vec3& _coord, const glm::vec2& _normal, int _numCorners, bool _isBeginning, Mesh& _mesh, float _width) {

    float v = _isBeginning ? 0.f : 1.f; // length-wise tex coord

    if (_numCorners < 1) {
        // "Butt" cap needs no extra vertices
        return;
    } 
    else if (_numCorners == 2) {
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

Mesh toSpline(const std::vector<glm::vec3>& _polyline, float _width, JoinType _join, CapType _cap, float _miterLimit) { //}, bool _close) {;

    Mesh mesh;
    size_t startIndex = 0;
    size_t endIndex = _polyline.size();
    bool endCap = true;

    float distance = 0; // Cumulative distance along the polyline.

    size_t origLineSize = _polyline.size();

    // endIndex/startIndex could be wrapped values, calculate lineSize accordingly
    int lineSize = (int)((endIndex > startIndex) ?
                   (endIndex - startIndex) :
                   (origLineSize - startIndex + endIndex));
    if (lineSize < 2) { return mesh; }

    glm::vec3 coordCurr(_polyline[startIndex]);
    // get the Point using wrapped index in the original line geometry
    glm::vec3 coordNext(_polyline[(startIndex + 1) % origLineSize]);
    glm::vec2 normPrev, normNext, miterVec;

    int cornersOnCap = (int)_cap;
    int trianglesOnJoin = (int)_join;

    // Process first point in line with an end cap
    normNext = glm::normalize(perp2d(coordCurr, coordNext));

    if (endCap)
        addCap(coordCurr, normNext, cornersOnCap, true, mesh, _width);
    
    addPolyLineVertex(coordCurr, normNext, {1.0f, 0.0f}, mesh, _width); // right corner
    addPolyLineVertex(coordCurr, -normNext, {0.0f, 0.0f}, mesh, _width); // left corner


    // Process intermediate points
    for (int i = 1; i < lineSize - 1; i++) {
        // get the Point using wrapped index in the original line geometry
        int nextIndex = (i + startIndex + 1) % origLineSize;

        distance += glm::distance(coordCurr, coordNext);

        coordCurr = coordNext;
        coordNext.x = _polyline[nextIndex].x;
        coordNext.y = _polyline[nextIndex].y;

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

            addPolyLineVertex(coordCurr, miterVec, {1.0, v}, mesh, _width); // right corner
            addPolyLineVertex(coordCurr, -miterVec, {0.0, v}, mesh, _width); // left corner
            indexPairs(1, mesh);

        }
        else {

            // Join type is a fan of triangles

            bool isRightTurn = (normNext.x * normPrev.y - normNext.y * normPrev.x) > 0; // z component of cross(normNext, normPrev)

            if (isRightTurn) {

                addPolyLineVertex(coordCurr, miterVec, {1.0f, v}, mesh, _width); // right (inner) corner
                addPolyLineVertex(coordCurr, -normPrev, {0.0f, v}, mesh, _width); // left (outer) corner
                indexPairs(1, mesh);

                addFan(coordCurr, -normPrev, -normNext, miterVec, {0.f, v}, {0.f, v}, {1.f, v}, trianglesOnJoin, mesh, _width);

                addPolyLineVertex(coordCurr, miterVec, {1.0f, v}, mesh, _width); // right (inner) corner
                addPolyLineVertex(coordCurr, -normNext, {0.0f, v}, mesh, _width); // left (outer) corner

            } else {

                addPolyLineVertex(coordCurr, normPrev, {1.0f, v}, mesh, _width); // right (outer) corner
                addPolyLineVertex(coordCurr, -miterVec, {0.0f, v}, mesh, _width); // left (inner) corner
                indexPairs(1, mesh);

                addFan(coordCurr, normPrev, normNext, -miterVec, {1.f, v}, {1.f, v}, {0.0f, v}, trianglesOnJoin, mesh, _width);

                addPolyLineVertex(coordCurr, normNext, {1.0f, v}, mesh, _width); // right (outer) corner
                addPolyLineVertex(coordCurr, -miterVec, {0.0f, v}, mesh, _width); // left (inner) corner
            }
        }
    }

    distance += glm::distance(coordCurr, coordNext);

    // Process last point in line with a cap
    addPolyLineVertex(coordNext, normNext, {1.f, distance}, mesh, _width); // right corner
    addPolyLineVertex(coordNext, -normNext, {0.f, distance}, mesh, _width); // left corner
    indexPairs(1, mesh);
    if (endCap)
        addCap(coordNext, normNext, cornersOnCap, false, mesh, _width);

    return mesh;
}

Mesh toTube(const Polyline& _polyline, const float* _array1D, int _n, int _resolution, bool _caps) {
    Mesh mesh;
    size_t offset = 0;


    if ( !_polyline.isClosed() && _caps) {
        mesh.addVertex(_polyline.getVertices()[0]);
        mesh.addNormal( glm::normalize( _polyline.getVertices()[1] - _polyline.getVertices()[2] ) );
        mesh.addTexCoord( glm::vec2(0.5, 0.0));
        offset = 1;
    }
    
    for (size_t i = 0; i < _polyline.size(); i++) {
        const glm::vec3& p0 = _polyline[i];
        const glm::vec3& n0 = _polyline.getNormalAtIndex(i);
        const glm::vec3& t0 = _polyline.getTangentAtIndex(i);
        float r0 = _array1D[i%_n];

        glm::vec3 v0;
        for(int j = 0; j < _resolution; j++) {
            float p = j / (float)_resolution;
            float a = p * TAU;
            v0 = glm::rotate(n0, a, t0);
            
            mesh.addTexCoord( glm::vec2(p, i/(_polyline.size()-1.0)) );
            mesh.addNormal(v0);
            
            v0 *= r0;
            v0 += p0;
            
            mesh.addVertex(v0);
        }
    }

    //--------------------------------------------------------------------------
    if ( !_polyline.isClosed() && _caps) {
        for (size_t i = 0; i < _resolution-1; i++)
            mesh.addTriangleIndices( 0, i + 2, i + 1);
        mesh.addTriangleIndices( 0, 1, _resolution);
    }
    
    size_t numOfTubeSections = _polyline.size();
    for (size_t y = 0; y < numOfTubeSections - 1; y++) {

        // 2 - 3
        // | \ |
        // 0 - 1

        for(int x = 0; x < _resolution-1; x++) {
            mesh.addTriangleIndices(y * _resolution + x + offset,
                                    y * _resolution + x+1 + offset,
                                    (y+1) * _resolution + x + offset);
            mesh.addTriangleIndices(y*_resolution + x+1 + offset,
                                    (y+1) * _resolution + x+1 + offset,
                                    (y+1)*_resolution + x + offset);
        }

        mesh.addTriangleIndices(y * _resolution + (_resolution-1) + offset,
                                y * _resolution + offset,
                                (y+1) * _resolution + (_resolution-1) + offset);
        mesh.addTriangleIndices(y * _resolution + offset,
                                (y+1) * _resolution + offset,
                                (y+1)*_resolution + (_resolution-1) + offset);
    }

    if ( !_polyline.isClosed() && _caps) {
        size_t vertsN = mesh.getVerticesTotal();
        mesh.addVertex(_polyline.getVertices()[_polyline.size()-1]);
        mesh.addTexCoord( glm::vec2(.5, 1.0) );
        mesh.addNormal( glm::normalize( _polyline.getVertices()[_polyline.size()-2] - _polyline.getVertices()[_polyline.size()-1] ) );

        for (size_t i = 0; i < _resolution; i++)
            mesh.addTriangleIndices( vertsN, vertsN - i - 2, vertsN - i - 1);

        mesh.addTriangleIndices( vertsN, vertsN - 1 , vertsN - _resolution);
    }

    return mesh;
}

std::vector<Line>   toLines(const BoundingBox& _bbox) {
    std::vector<Line> lines;

    //    D ---- A
    // C ---- B  |
    // |  |   |  |
    // |  I --|- F
    // H .... G

    glm::vec3 A = _bbox.max;
    glm::vec3 H = _bbox.min;

    glm::vec3 B = glm::vec3(A.x, A.y, H.z);
    glm::vec3 C = glm::vec3(H.x, A.y, H.z);
    glm::vec3 D = glm::vec3(H.x, A.y, A.z);

    glm::vec3 F = glm::vec3(A.x, H.y, A.z);
    glm::vec3 G = glm::vec3(A.x, H.y, H.z);
    glm::vec3 I = glm::vec3(H.x, H.y, A.z);

    lines.push_back( Line(A, F) );
    lines.push_back( Line(A, B) );
    lines.push_back( Line(A, D) );

    lines.push_back( Line(B, G) );
    lines.push_back( Line(B, C) );

    lines.push_back( Line(C, H) );
    lines.push_back( Line(C, D) );
    
    lines.push_back( Line(D, I) );

    lines.push_back( Line(F, I) );
    lines.push_back( Line(F, G) );

    lines.push_back( Line(G, H) );
    lines.push_back( Line(H, I) );

    return lines;
}

std::vector<Line>   toLines(const std::vector<Triangle>& _triangles) {
    std::vector<Line> lines;
    
    for (size_t i = 0; i < _triangles.size(); i++) {
        Line l1 = Line(_triangles[i][0], _triangles[i][1]);
        Line l2 = Line(_triangles[i][1], _triangles[i][2]);
        Line l3 = Line(_triangles[i][2], _triangles[i][0]);

        if (_triangles[i].haveColors()) {
            l1.setColor(0, _triangles[i].getColor(0));
            l1.setColor(1, _triangles[i].getColor(1));
            l2.setColor(0, _triangles[i].getColor(1));
            l2.setColor(1, _triangles[i].getColor(2));
            l3.setColor(0, _triangles[i].getColor(2));
            l3.setColor(1, _triangles[i].getColor(0));
        }
        lines.push_back(l1);
        lines.push_back(l2);
        lines.push_back(l3);
    }

    // TODO:
    //      - REMOVE DUPLICATES
    //
    return lines;
}

}