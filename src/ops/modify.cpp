#include "hilma/ops/modify.h"
#include "hilma/ops/earcut.h"

#include "hilma/math.h"
#include "hilma/types/BoundingBox.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/norm.hpp"

namespace mapbox { namespace util {

template <>
struct nth<0, glm::vec2> {
    inline static float get(const glm::vec2 &t) { return t.x; };
};
template <>
struct nth<1, glm::vec2> {
    inline static float get(const glm::vec2 &t) { return t.y; };
};

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

template <typename T>
Mesh Modify::surface(const std::vector<std::vector<T>>& _polygon) {
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
        mesh.addTexCoord(   mapValue(p.x, bb.min.x, bb.max.x, 0.0f, 1.0f, true),
                            mapValue(p.y, bb.min.y, bb.max.y, 0.0f, 1.0f, true) );
    }

    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(_polygon);
    for (size_t i = 0; i < indices.size(); i++)
        mesh.addIndex(indices[i]);

    return mesh;
}

template Mesh Modify::surface<glm::vec2>(const std::vector<std::vector<glm::vec2>>&); 
template Mesh Modify::surface<glm::vec3>(const std::vector<std::vector<glm::vec3>>&); 

template <typename T>
Mesh Modify::extrude(const std::vector<std::vector<T>>& _polygon, float _maxHeight, float _minHeight) {
    Mesh mesh;
    
    static const glm::vec3 upVector(0.0f, 0.0f, 1.0f);
    glm::vec3 normalVector;

    int lineN  = 0;
    INDEX_TYPE vertexN = 0;
    for (auto& line : _polygon) {
        size_t lineSize = line.size();

        for (size_t i = 0; i < lineSize - 1; i++) {

            glm::vec3 a(line[i].x, line[i].y, 0.f);
            glm::vec3 b(line[i+1].x, line[i+1].y, 0.f);

            
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

        lineN++;
    }

    return mesh;
}

template Mesh Modify::extrude<glm::vec2>(const std::vector<std::vector<glm::vec2>>&, float, float); 
template Mesh Modify::extrude<glm::vec3>(const std::vector<std::vector<glm::vec3>>&, float, float); 

// From Tangram
// https://github.com/tangrams/tangram-es/blob/e4a323afeb310520456aec49e338614120a7ffa2/core/src/util/Modifys.cpp

// Get 2D perpendicular of two points

inline glm::vec2 perp2d(const glm::vec2& _v1, const glm::vec2& _v2 ) {
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
void indexPairs(size_t _nPairs, Mesh& _mesh) {
    size_t nVertices = _mesh.getVerticesTotal();

    for (size_t i = 0; i < _nPairs; i++) {
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

template <typename T>
Mesh Modify::spline(const std::vector<T>& _polyline, float _width, JoinType _join, CapType _cap, float _miterLimit) { //}, bool _close) {;

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

    glm::vec2 coordCurr(_polyline[startIndex].x, 
                        _polyline[startIndex].y);
    // get the Point using wrapped index in the original line geometry
    glm::vec2 coordNext(_polyline[(startIndex + 1) % origLineSize].x, 
                        _polyline[(startIndex + 1) % origLineSize].y);
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

template Mesh Modify::spline<glm::vec2>(const std::vector<glm::vec2>&, float, JoinType, CapType, float);
template Mesh Modify::spline<glm::vec3>(const std::vector<glm::vec3>&, float, JoinType, CapType, float);

Mesh tube(const Polyline& _polyline, float _width, int _resolution) {
    Mesh mesh;
    
    for (size_t i = 0; i < _polyline.size(); i++) {
        const glm::vec3& p0 = _polyline.getVertices()[i];
        const glm::vec3& n0 = _polyline.getNormalAtIndex(i);
        const glm::vec3& t0 = _polyline.getTangentAtIndex(i);
        // float r0 = tubeRadius[i];
        float r0 = _width;

        glm::vec3 v0;
        for(int j = 0; j < _resolution; j++) {
            float p = j / (float)_resolution;
            float a = p * TAU;
            v0 = glm::rotate(n0, a, t0);
            
            mesh.addNormal(v0);
            
            v0 *= r0;
            v0 += p0;
            
            mesh.addVertex(v0);
        }
    }

    //--------------------------------------------------------------------------
    // std::vector<glm::vec3>& verts = mesh.points;
    size_t numOfVerts = mesh.getVerticesTotal();
    size_t i0, i1, i2;
    bool bLeftToRight;
    bool bRingEnd;
    int k;
    
    size_t numOfTubeSections = _polyline.size();
    for (size_t y = 0; y < numOfTubeSections - 1; y++) {

        // 2 - 3
        // | \ |
        // 0 - 1

        for(int x = 0; x < _resolution; x++) {
            mesh.addIndex( y * _resolution + x);

            if (x+1 > _resolution-1)
                mesh.addIndex( y * _resolution + x+1 - _resolution);
            else
                mesh.addIndex( y * _resolution + x+1);

            mesh.addIndex( (y+1) * _resolution + x);

            // mesh.addIndex( (y)*_resolution + x+1 );
            if (x+1 > _resolution-1)
                mesh.addIndex( y * _resolution + x+1 - _resolution);
            else
                mesh.addIndex( y*_resolution + x+1 );

            // mesh.addIndex( (y+1)*_resolution + x+1 );
            if (x+1 > _resolution-1)
                mesh.addIndex( (y+1) * _resolution + x+1 - _resolution);
            else
                mesh.addIndex( (y+1) * _resolution + x+1);

            mesh.addIndex( (y+1)*_resolution + x );
        }
    }

    return mesh;
}

}