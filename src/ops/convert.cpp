#include "hilma/ops/convert.h"
#include "hilma/ops/earcut.h"

#include "hilma/math.h"
#include "hilma/types/BoundingBox.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/norm.hpp"
#include <glm/gtx/hash.hpp>

#include <map>
#include <unordered_map>

#include <algorithm>

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
Mesh Convert::toSurface(const std::vector<std::vector<T>>& _polygon) {
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

template Mesh Convert::toSurface<glm::vec2>(const std::vector<std::vector<glm::vec2>>&); 
template Mesh Convert::toSurface<glm::vec3>(const std::vector<std::vector<glm::vec3>>&); 

template <typename T>
Mesh Convert::toWall(const std::vector<std::vector<T>>& _polygon, float _maxHeight, float _minHeight) {
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
                mesh.addTriangleIndices(vertexN, vertexN + 2, vertexN + 1);
                mesh.addTriangleIndices(vertexN + 1, vertexN + 2, vertexN + 3);
            }
            else {
                mesh.addTriangleIndices(vertexN, vertexN + 1, vertexN + 2);
                mesh.addTriangleIndices(vertexN + 1, vertexN + 3, vertexN + 2);
            }

            vertexN += 4;
        }

        lineN++;
    }

    return mesh;
}

template Mesh Convert::toWall<glm::vec2>(const std::vector<std::vector<glm::vec2>>&, float, float); 
template Mesh Convert::toWall<glm::vec3>(const std::vector<std::vector<glm::vec3>>&, float, float); 

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
        _mesh.addTriangleIndices(  startIndex, // center vertex
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

template <typename T>
Mesh Convert::toSpline(const std::vector<T>& _polyline, float _width, JoinType _join, CapType _cap, float _miterLimit) { //}, bool _close) {;

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

template Mesh Convert::toSpline<glm::vec2>(const std::vector<glm::vec2>&, float, JoinType, CapType, float);
template Mesh Convert::toSpline<glm::vec3>(const std::vector<glm::vec3>&, float, JoinType, CapType, float);

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

Mesh toMeshEdges(const BoundingBox& _bbox) {
    std::vector<Line> lines = toLines(_bbox);

    Mesh mesh;
    mesh.setEdgeType( LINES );
    mesh.addEdges(&lines[0], lines.size());
    return mesh;
}


//  Triangulator by Michael Fogleman ( @FogleBird )
//  https://github.com/fogleman/hmm/blob/master/src/triangulator.cpp
//  All code and credits are for his genius. I took the code and make 
//  it significantly uglier using lambdas for my own conviniance
//

std::pair<glm::ivec2, float> FindCandidate( const Image& _image, const glm::ivec2 p0, const glm::ivec2 p1, const glm::ivec2 p2) { 
    
    const auto edge = []( const glm::ivec2 a, const glm::ivec2 b, const glm::ivec2 c) {
        return (b.x - c.x) * (a.y - c.y) - (b.y - c.y) * (a.x - c.x);
    };

    // triangle bounding box
    const glm::ivec2 min = glm::min(glm::min(p0, p1), p2);
    const glm::ivec2 max = glm::max(glm::max(p0, p1), p2);

    // forward differencing variables
    int w00 = edge(p1, p2, min);
    int w01 = edge(p2, p0, min);
    int w02 = edge(p0, p1, min);
    const int a01 = p1.y - p0.y;
    const int b01 = p0.x - p1.x;
    const int a12 = p2.y - p1.y;
    const int b12 = p1.x - p2.x;
    const int a20 = p0.y - p2.y;
    const int b20 = p2.x - p0.x;

    // pre-multiplied z values at vertices
    const float a = edge(p0, p1, p2);
    const float z0 = _image.getData(p0.x, p0.y) / a;
    const float z1 = _image.getData(p1.x, p1.y) / a;
    const float z2 = _image.getData(p2.x, p2.y) / a;

    // iterate over pixels in bounding box
    float maxError = 0;
    glm::ivec2 maxPoint(0);
    for (int y = min.y; y <= max.y; y++) {
        // compute starting offset
        int dx = 0;
        if (w00 < 0 && a12 != 0)
            dx = std::max(dx, -w00 / a12);
        if (w01 < 0 && a20 != 0)
            dx = std::max(dx, -w01 / a20);
        if (w02 < 0 && a01 != 0)
            dx = std::max(dx, -w02 / a01);

        int w0 = w00 + a12 * dx;
        int w1 = w01 + a20 * dx;
        int w2 = w02 + a01 * dx;

        bool wasInside = false;

        for (int x = min.x + dx; x <= max.x; x++) {
            // check if inside triangle
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                wasInside = true;

                // compute z using barycentric coordinates
                const float z = z0 * w0 + z1 * w1 + z2 * w2;
                const float dz = std::abs(z - _image.getData(x, y));
                if (dz > maxError) {
                    maxError = dz;
                    maxPoint = glm::ivec2(x, y);
                }
            } else if (wasInside) {
                break;
            }

            w0 += a12;
            w1 += a20;
            w2 += a01;
        }

        w00 += b12;
        w01 += b20;
        w02 += b01;
    }

    if (maxPoint == p0 || maxPoint == p1 || maxPoint == p2) {
        maxError = 0;
    }

    return std::make_pair(maxPoint, maxError);
}

struct TriangulatorData {
    std::vector<glm::ivec2>  points;
    std::vector<int>        triangles;
    std::vector<int>        halfedges;

    std::vector<glm::ivec2>  candidates;
    std::vector<float>      errors;
    std::vector<int>        queueIndexes;
    std::vector<int>        queue;
    std::vector<int>        pending;

    void QueueSwap(const int i, const int j) {
        const int pi = queue[i];
        const int pj = queue[j];
        queue[i] = pj;
        queue[j] = pi;
        queueIndexes[pi] = j;
        queueIndexes[pj] = i;
    };

    bool QueueLess(const int i, const int j) {
        return -errors[queue[i]] < -errors[queue[j]];
    };

    bool QueueDown(const int i0, const int n) {
        int i = i0;
        while (1) {
            const int j1 = 2 * i + 1;
            if (j1 >= n || j1 < 0) {
                break;
            }
            const int j2 = j1 + 1;
            int j = j1;
            if (j2 < n && QueueLess(j2, j1)) {
                j = j2;
            }
            if (!QueueLess(j, i)) {
                break;
            }
            QueueSwap(i, j);
            i = j;
        }
        return i > i0;
    }

    int QueuePopBack() {
        const int t = queue.back();
        queue.pop_back();
        queueIndexes[t] = -1;
        return t;
    }

    int QueuePop() {
        const int n = queue.size() - 1;
        QueueSwap(0, n);
        QueueDown(0, n);
        return QueuePopBack();
    }

    void QueueUp(const int j0) {
        int j = j0;
        while (1) {
            int i = (j - 1) / 2;
            if (i == j || !QueueLess(j, i)) {
                break;
            }
            QueueSwap(i, j);
            j = i;
        }
    }

    void QueuePush(const int t) {
        const int i = queue.size();
        queueIndexes[t] = i;
        queue.push_back(t);
        QueueUp(i);
    }

    void QueueRemove(const int t) {
        const int i = queueIndexes[t];
        if (i < 0) {
            const auto it = std::find(pending.begin(), pending.end(), t);
            if (it != pending.end()) {
                std::swap(*it, pending.back());
                pending.pop_back();
            } 
            else {
                // this shouldn't happen!
            }
            return;
        }
        const int n = queue.size() - 1;
        if (n != i) {
            QueueSwap(i, n);
            if (!QueueDown(i, n)) {
                QueueUp(i);
            }
        }
        QueuePopBack();
    }

    int AddTriangle(const int a, const int b, const int c,
                    const int ab, const int bc, const int ca,
                    int e) {
        if (e < 0) {
            // new halfedge index
            e = triangles.size();
            // add triangle vertices
            triangles.push_back(a);
            triangles.push_back(b);
            triangles.push_back(c);
            // add triangle halfedges
            halfedges.push_back(ab);
            halfedges.push_back(bc);
            halfedges.push_back(ca);
            // add triangle metadata
            candidates.emplace_back(0);
            errors.push_back(0);
            queueIndexes.push_back(-1);
        } 
        else {
            // set triangle vertices
            triangles[e + 0] = a;
            triangles[e + 1] = b;
            triangles[e + 2] = c;
            // set triangle halfedges
            halfedges[e + 0] = ab;
            halfedges[e + 1] = bc;
            halfedges[e + 2] = ca;
        }

        // link neighboring halfedges
        if (ab >= 0)
            halfedges[ab] = e + 0;
        if (bc >= 0)
            halfedges[bc] = e + 1;
        if (ca >= 0)
            halfedges[ca] = e + 2;

        // add triangle to pending queue for later rasterization
        const int t = e / 3;
        pending.push_back(t);

        // return first halfedge index
        return e;
    }

    int AddPoint(const glm::ivec2& _point) {
        const int i = points.size();
        points.push_back(_point);
        return i;
    }

    void Flush( const Image& _image) {
        for (const int t : pending) {

            // rasterize triangle to find maximum pixel error
            const auto pair = FindCandidate(_image,
                                            points[ triangles[t*3+0] ],
                                            points[ triangles[t*3+1] ],
                                            points[ triangles[t*3+2] ]);

            // update metadata
            candidates[t] = pair.first;
            errors[t] = pair.second;

            // add triangle to priority queue
            QueuePush(t);
        }

        pending.clear();
    };

    void Legalize(const int a) {
        
        // if the pair of triangles doesn't satisfy the Delaunay condition
        // (p1 is inside the circumcircle of [p0, pl, pr]), flip them,
        // then do the same check/flip recursively for the new pair of triangles
        //
        //           pl                    pl
        //          /||\                  /  \
        //       al/ || \bl            al/    \a
        //        /  ||  \              /      \
        //       /  a||b  \    flip    /___ar___\
        //     p0\   ||   /p1   =>   p0\---bl---/p1
        //        \  ||  /              \      /
        //       ar\ || /br             b\    /br
        //          \||/                  \  /
        //           pr                    pr

        const auto inCircle = [](
            const glm::ivec2 a, const glm::ivec2 b, const glm::ivec2 c,
            const glm::ivec2 p)
        {
            const int64_t dx = a.x - p.x;
            const int64_t dy = a.y - p.y;
            const int64_t ex = b.x - p.x;
            const int64_t ey = b.y - p.y;
            const int64_t fx = c.x - p.x;
            const int64_t fy = c.y - p.y;
            const int64_t ap = dx * dx + dy * dy;
            const int64_t bp = ex * ex + ey * ey;
            const int64_t cp = fx * fx + fy * fy;
            return dx*(ey*cp-bp*fy)-dy*(ex*cp-bp*fx)+ap*(ex*fy-ey*fx) < 0;
        };

        const int b = halfedges[a];

        if (b < 0) {
            return;
        }

        const int a0 = a - a % 3;
        const int b0 = b - b % 3;
        const int al = a0 + (a + 1) % 3;
        const int ar = a0 + (a + 2) % 3;
        const int bl = b0 + (b + 2) % 3;
        const int br = b0 + (b + 1) % 3;
        const int p0 = triangles[ar];
        const int pr = triangles[a];
        const int pl = triangles[al];
        const int p1 = triangles[bl];

        if (!inCircle(points[p0], points[pr], points[pl], points[p1])) {
            return;
        }

        const int hal = halfedges[al];
        const int har = halfedges[ar];
        const int hbl = halfedges[bl];
        const int hbr = halfedges[br];

        QueueRemove(a / 3);
        QueueRemove(b / 3);

        const int t0 = AddTriangle(p0, p1, pl, -1, hbl, hal, a0);
        const int t1 = AddTriangle(p1, p0, pr, t0, har, hbr, b0);

        Legalize(t0 + 1);
        Legalize(t1 + 2);
    }
};


Mesh toTerrain( const Image& _image,
                const float _zScale,
                const float _maxError, const float _baseHeight, 
                const int _maxTriangles, const int _maxPoints) {

    if (_image.getChannels() != 1)
        return Mesh();

    TriangulatorData data;

    // add points at all four corners
    const int x0 = 0;
    const int y0 = 0;
    const int x1 = _image.getWidth() - 1;
    const int y1 = _image.getHeight() - 1;
    const int p0 = data.AddPoint(glm::ivec2(x0, y0));
    const int p1 = data.AddPoint(glm::ivec2(x1, y0));
    const int p2 = data.AddPoint(glm::ivec2(x0, y1));
    const int p3 = data.AddPoint(glm::ivec2(x1, y1));

    // add initial two triangles
    const int t0 = data.AddTriangle(p3, p0, p2, -1, -1, -1, -1);
    data.AddTriangle(p0, p3, p1, t0, -1, -1, -1);
    data.Flush(_image);

    // helper function to check if triangulation is complete
    const auto done = [&]() {
        const float e = data.errors[data.queue[0]];
        if (e <= _maxError) {
            return true;
        }
        if (_maxTriangles > 0 && data.queue.size() >= _maxTriangles) {
            return true;
        }
        if (_maxPoints > 0 && data.points.size() >= _maxPoints) {
            return true;
        }
        return e == 0;
    };

    while (!done()) {
        // pop triangle with highest error from priority queue
        const int t = data.QueuePop();

        const int e0 = t * 3 + 0;
        const int e1 = t * 3 + 1;
        const int e2 = t * 3 + 2;

        const int p0 = data.triangles[e0];
        const int p1 = data.triangles[e1];
        const int p2 = data.triangles[e2];

        const glm::ivec2 a = data.points[p0];
        const glm::ivec2 b = data.points[p1];
        const glm::ivec2 c = data.points[p2];
        const glm::ivec2 p = data.candidates[t];

        const int pn = data.AddPoint(p);

        const auto collinear = []( const glm::ivec2 p0, const glm::ivec2 p1, const glm::ivec2 p2) {
            return (p1.y-p0.y)*(p2.x-p1.x) == (p2.y-p1.y)*(p1.x-p0.x);
        };

        const auto handleCollinear = [&](const int pn, const int a) {
            const int a0 = a - a % 3;
            const int al = a0 + (a + 1) % 3;
            const int ar = a0 + (a + 2) % 3;
            const int p0 = data.triangles[ar];
            const int pr = data.triangles[a];
            const int pl = data.triangles[al];
            const int hal = data.halfedges[al];
            const int har = data.halfedges[ar];

            const int b = data.halfedges[a];

            if (b < 0) {
                const int t0 = data.AddTriangle(pn, p0, pr, -1, har, -1, a0);
                const int t1 = data.AddTriangle(p0, pn, pl, t0, -1, hal, -1);
                data.Legalize(t0 + 1);
                data.Legalize(t1 + 2);
                return;
            }

            const int b0 = b - b % 3;
            const int bl = b0 + (b + 2) % 3;
            const int br = b0 + (b + 1) % 3;
            const int p1 = data.triangles[bl];
            const int hbl = data.halfedges[bl];
            const int hbr = data.halfedges[br];

            data.QueueRemove(b / 3);

            const int t0 = data.AddTriangle(p0, pr, pn, har, -1, -1, a0);
            const int t1 = data.AddTriangle(pr, p1, pn, hbr, -1, t0 + 1, b0);
            const int t2 = data.AddTriangle(p1, pl, pn, hbl, -1, t1 + 1, -1);
            const int t3 = data.AddTriangle(pl, p0, pn, hal, t0 + 2, t2 + 1, -1);

            data.Legalize(t0);
            data.Legalize(t1);
            data.Legalize(t2);
            data.Legalize(t3);
        };

        if (collinear(a, b, p))
            handleCollinear(pn, e0);
        else if (collinear(b, c, p))
            handleCollinear(pn, e1);
        else if (collinear(c, a, p))
            handleCollinear(pn, e2);
        else {
            const int h0 = data.halfedges[e0];
            const int h1 = data.halfedges[e1];
            const int h2 = data.halfedges[e2];

            const int t0 = data.AddTriangle(p0, p1, pn, h0, -1, -1, e0);
            const int t1 = data.AddTriangle(p1, p2, pn, h1, -1, t0 + 1, -1);
            const int t2 = data.AddTriangle(p2, p0, pn, h2, t0 + 2, t1 + 1, -1);

            data.Legalize(t0);
            data.Legalize(t1);
            data.Legalize(t2);
        }

        data.Flush(_image);
    }

    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> points;
    points.reserve(data.points.size());
    texcoords.reserve(data.points.size());
    const int w = _image.getWidth();
    const int h = _image.getHeight();
    const int w1 = w - 1;
    const int h1 = h - 1;

    for (const glm::ivec2 &p : data.points) {
        points.emplace_back(p.x, h1 - p.y, _image.getData(p.x, p.y) * _zScale);
        texcoords.emplace_back(p.x/float(w1), 1.0f-p.y/float(h1));
    }

    std::vector<glm::ivec3> triangles;
    triangles.reserve(data.queue.size());
    for (const int i : data.queue) {
        triangles.emplace_back(
            data.triangles[i * 3 + 0],
            data.triangles[i * 3 + 1],
            data.triangles[i * 3 + 2] );
    }

    // BASE
    //
    
    if ( _baseHeight > 0.0f ) {

        const float z = -_baseHeight * _zScale;
        
        std::map<int, float> x0s;
        std::map<int, float> x1s;
        std::map<int, float> y0s;
        std::map<int, float> y1s;
        std::unordered_map<glm::vec3, int> lookup;

        // find points along each edge
        for (int i = 0; i < points.size(); i++) {
            const auto &p = points[i];
            bool edge = false;

            if (p.x == 0) {
                x0s[p.y] = p.z;
                edge = true;
            }
            else if (p.x == w1) {
                x1s[p.y] = p.z;
                edge = true;
            }

            if (p.y == 0) {
                y0s[p.x] = p.z;
                edge = true;
            }
            else if (p.y == h1) {
                y1s[p.x] = p.z;
                edge = true;
            }

            if (edge)
                lookup[p] = i;
        }

        std::vector<std::pair<int, float>> sx0s(x0s.begin(), x0s.end());
        std::vector<std::pair<int, float>> sx1s(x1s.begin(), x1s.end());
        std::vector<std::pair<int, float>> sy0s(y0s.begin(), y0s.end());
        std::vector<std::pair<int, float>> sy1s(y1s.begin(), y1s.end());

        const auto pointIndex = [&lookup, &points, &texcoords, &w1, &h1](
            const float x, const float y, const float z)
        {
            const glm::vec3 point(x, y, z);
            if (lookup.find(point) == lookup.end()) {
                lookup[point] = points.size();
                points.push_back(point);
                texcoords.push_back( glm::vec2(x/float(w1), y/float(h1)) );
            }
            return lookup[point];
        };

        // compute base center point
        const int center = pointIndex(w * 0.5f, h * 0.5f, z);

        // edge x = 0
        for (int i = 1; i < sx0s.size(); i++) {
            const int y0 = sx0s[i-1].first;
            const int y1 = sx0s[i].first;
            const float z0 = sx0s[i-1].second;
            const float z1 = sx0s[i].second;
            const int p00 = pointIndex(0, y0, z);
            const int p01 = pointIndex(0, y0, z0);
            const int p10 = pointIndex(0, y1, z);
            const int p11 = pointIndex(0, y1, z1);
            triangles.emplace_back(p01, p10, p00);
            triangles.emplace_back(p01, p11, p10);
            triangles.emplace_back(center, p00, p10);
        }

        // edge x = w1
        for (int i = 1; i < sx1s.size(); i++) {
            const int y0 = sx1s[i-1].first;
            const int y1 = sx1s[i].first;
            const float z0 = sx1s[i-1].second;
            const float z1 = sx1s[i].second;
            const int p00 = pointIndex(w1, y0, z);
            const int p01 = pointIndex(w1, y0, z0);
            const int p10 = pointIndex(w1, y1, z);
            const int p11 = pointIndex(w1, y1, z1);
            triangles.emplace_back(p00, p10, p01);
            triangles.emplace_back(p10, p11, p01);
            triangles.emplace_back(center, p10, p00);
        }

        // edge y = 0
        for (int i = 1; i < sy0s.size(); i++) {
            const int x0 = sy0s[i-1].first;
            const int x1 = sy0s[i].first;
            const float z0 = sy0s[i-1].second;
            const float z1 = sy0s[i].second;
            const int p00 = pointIndex(x0, 0, z);
            const int p01 = pointIndex(x0, 0, z0);
            const int p10 = pointIndex(x1, 0, z);
            const int p11 = pointIndex(x1, 0, z1);
            triangles.emplace_back(p00, p10, p01);
            triangles.emplace_back(p10, p11, p01);
            triangles.emplace_back(center, p10, p00);
        }

        // edge y = h1
        for (int i = 1; i < sy1s.size(); i++) {
            const int x0 = sy1s[i-1].first;
            const int x1 = sy1s[i].first;
            const float z0 = sy1s[i-1].second;
            const float z1 = sy1s[i].second;
            const int p00 = pointIndex(x0, h1, z);
            const int p01 = pointIndex(x0, h1, z0);
            const int p10 = pointIndex(x1, h1, z);
            const int p11 = pointIndex(x1, h1, z1);
            triangles.emplace_back(p01, p10, p00);
            triangles.emplace_back(p01, p11, p10);
            triangles.emplace_back(center, p00, p10);
        }
    }

    Mesh mesh;

    for (const glm::vec3 &p : points)
        mesh.addVertex( p );

    for (const glm::vec2 &t : texcoords)
        mesh.addTexCoord( t );
    
    for (const glm::ivec3 &tri : triangles)
        mesh.addTriangleIndices( tri[0], tri[1], tri[2] );

    return mesh;
}


}