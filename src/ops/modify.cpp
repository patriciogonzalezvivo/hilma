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
}}

namespace hilma {

Mesh surface(const Polyline& _polyline) {
    return surface(_polyline.m_points);
}

Mesh surface(const std::vector<glm::vec2>& _contour) {
    std::vector<std::vector<glm::vec2>> polygon;
    polygon.push_back(_contour);
    return surface(polygon);
}

Mesh surface(const std::vector<std::vector<glm::vec2>>& _polygon) {
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

Mesh extrude(const Polyline& _polyline, float _maxHeight, float _minHeight) {
    return extrude(_polyline.m_points, _maxHeight, _minHeight);
}

Mesh extrude(const std::vector<glm::vec2>& _contour, float _maxHeight, float _minHeight) {
    std::vector<std::vector<glm::vec2>> polygon;
    polygon.push_back(_contour);
    return extrude(polygon, _maxHeight, _minHeight);
}

Mesh extrude(const std::vector<std::vector<glm::vec2>>& _polygon, float _maxHeight, float _minHeight) {
    Mesh mesh;
    
    static const glm::vec3 upVector(0.0f, 0.0f, 1.0f);
    glm::vec3 normalVector;

    int lineN  = 0;
    INDEX_TYPE vertexN = 0;
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

        lineN++;
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

Mesh spline(const Polyline& _polyline, float _width, JoinType _join, CapType _cap, float _miterLimit) {
    return spline(_polyline.m_points, _width, _join, _cap, _miterLimit);
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