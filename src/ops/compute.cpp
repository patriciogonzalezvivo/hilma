#include "hilma/ops/compute.h"

#include <algorithm>
#include <map>

#include "hilma/text.h"

namespace hilma {

//This is for polygon/contour simplification - we use it to reduce the number of m_points needed in
//representing the letters as openGL shapes - will soon be moved to ofGraphics.cpp

// From: http://softsurfer.com/Archive/algorithm_0205/algorithm_0205.htm
// Copyright 2002, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

typedef struct{
    glm::vec2 P0, P1;
} Segment;

#define norm2(v)   glm::dot(v,v)        // norm2 = squared length of vector
#define norm(v)    sqrt(norm2(v))  // norm = length of vector
#define d2(u,v)    norm2(u-v)      // distance squared = norm2 of difference
#define d(u,v)     norm(u-v)       // distance = norm of difference

//--------------------------------------------------
static void simplifyDP(float tol, glm::vec2* v, int j, int k, int* mk ) {
    if (k <= j+1) // there is nothing to simplify
        return;

    // check for adequate approximation by segment S from v[j] to v[k]
    int     maxi  = j;          // index of vertex farthest from S
    float   maxd2 = 0;         // distance squared of farthest vertex
    float   tol2  = tol * tol;  // tolerance squared
    Segment S   = {v[j], v[k]};  // segment from v[j] to v[k]
    glm::vec2 u;
    u       = S.P1 - S.P0;   // segment direction vector
    float  cu   = glm::dot(u,u);     // segment length squared

    // test each vertex v[i] for max distance from S
    // compute using the Feb 2001 Algorithm's dist_ofPoint_to_Segment()
    // Note: this works in any dimension (2D, 3D, ...)
    glm::vec2  w;
    glm::vec2   Pb;                // base of perpendicular from v[i] to S
    float  b, cw, dv2;        // dv2 = distance v[i] to S squared

    for (int i=j+1; i<k; i++) {
        // compute distance squared
        w = v[i] - S.P0;
        cw = glm::dot(w,u);
        if ( cw <= 0 ) dv2 = d2(v[i], S.P0);
        else if ( cu <= cw ) dv2 = d2(v[i], S.P1);
        else {
            b = (float)(cw / cu);
            Pb = S.P0 + u*b;
            dv2 = d2(v[i], Pb);
        }
        // test with current max distance squared
        if (dv2 <= maxd2) continue;

        // v[i] is a new max vertex
        maxi = i;
        maxd2 = dv2;
    }
    if (maxd2 > tol2)        // error is worse than the tolerance
    {
        // split the polyline at the farthest vertex from S
        mk[maxi] = 1;      // mark v[maxi] for the simplified polyline
        // recursively simplify the two subpolylines at v[maxi]
        simplifyDP( tol, v, j, maxi, mk );  // polyline v[j] to v[maxi]
        simplifyDP( tol, v, maxi, k, mk );  // polyline v[maxi] to v[k]
    }
    // else the approximation is OK, so ignore intermediate vertices
    return;
}

std::vector<glm::vec2> getSimplify(const std::vector<glm::vec2>& _points, float _tolerance) {
    std::vector<glm::vec2> rta;
    rta.assign(_points.begin(),_points.end());
    simplify(rta, _tolerance);
    return rta;
}

void simplify(std::vector<glm::vec2>& _points, float _tolerance) {

    if (_points.size() < 2) return;

    int n = _points.size();

    if (n == 0) {
        return;
    }

    std::vector<glm::vec2> sV;
    sV.resize(n);

    int    i, k, m, pv;            // misc counters
    float  tol2 = _tolerance * _tolerance;       // tolerance squared
    std::vector<glm::vec2> vt;
    std::vector<int> mk;
    vt.resize(n);
    mk.resize(n,0);


    // STAGE 1.  Vertex Reduction within tolerance of prior vertex cluster
    vt[0] = _points[0];              // start at the beginning
    for (i=k=1, pv=0; i<n; i++) {
        if (d2(_points[i], _points[pv]) < tol2) continue;

        vt[k++] = _points[i];
        pv = i;
    }
    if (pv < n-1) vt[k++] = _points[n-1];      // finish at the end

    // STAGE 2.  Douglas-Peucker polyline simplification
    mk[0] = mk[k-1] = 1;       // mark the first and last vertices
    simplifyDP( _tolerance, &vt[0], 0, k-1, &mk[0] );

    // copy marked vertices to the output simplified polyline
    for (i=m=0; i<k; i++) {
        if (mk[i]) sV[m++] = vt[i];
    }

    //get rid of the unused points
    if ( m < (int)sV.size() ) {
        _points.assign( sV.begin(),sV.begin()+m );
    }else{
        _points = sV;
    }
}

bool lexicalComparison(const glm::vec2& _v1, const glm::vec2& _v2) {
    if (_v1.x > _v2.x) return true;
    else if (_v1.x < _v2.x) return false;
    else if (_v1.y > _v2.y) return true;
    else return false;
}

bool isRightTurn(const glm::vec2& _a, const glm::vec2& _b, const glm::vec2& _c) {
    // use the cross product to determin if we have a right turn
    return ((_b.x - _a.x)*(_c.y - _a.y) - (_b.y - _a.y)*(_c.x - _a.x)) > 0;
}

std::vector<glm::vec2> getConvexHull(const std::vector<glm::vec2>& _points) {
    std::vector<glm::vec2> pts;
    pts.assign(_points.begin(),_points.end());

    std::vector<glm::vec2> hull;
    glm::vec2 h1,h2,h3;

    if (_points.size() < 3) {
        // std::cout << "Error: you need at least three points to calculate the convex hull" << std::endl;
        return hull;
    }

    std::sort(pts.begin(), pts.end(), &lexicalComparison);

    hull.push_back(pts.at(0));
    hull.push_back(pts.at(1));

    size_t currentPoint = 2;
    int direction = 1;

    for (int i=0; i<3000; i++) { //max 1000 tries

        hull.push_back(pts.at(currentPoint));

        // look at the turn direction in the last three points
        h1 = hull.at(hull.size()-3);
        h2 = hull.at(hull.size()-2);
        h3 = hull.at(hull.size()-1);

        // while there are more than two points in the hull
        // and the last three points do not make a right turn
        while (!isRightTurn(h1, h2, h3) && hull.size() > 2) {

            // remove the middle of the last three points
            hull.erase(hull.end() - 2);

            if (hull.size() >= 3) {
                h1 = hull.at(hull.size()-3);
            }
            h2 = hull.at(hull.size()-2);
            h3 = hull.at(hull.size()-1);
        }

        // going through left-to-right calculates the top hull
        // when we get to the end, we reverse direction
        // and go back again right-to-left to calculate the bottom hull
        if (currentPoint == pts.size() -1 || currentPoint == 0) {
            direction = direction * -1;
        }

        currentPoint += direction;

        if (hull.front()==hull.back()) {
            if (currentPoint == 3 && direction == 1) {
                currentPoint = 4;
            } else {
                break;
            }
        }
    }

    return hull;
}



float getArea(const std::vector<glm::vec2>& _points) {
    float area = 0.0;

    for (int i=0;i<(int)_points.size()-1;i++) {
        area += _points[i].x * _points[i+1].y - _points[i+1].x * _points[i].y;
    }
    area += _points[_points.size()-1].x * _points[0].y - _points[0].x * _points[_points.size()-1].y;
    area *= 0.5;

    return area;
}

glm::vec2 getCentroid(const std::vector<glm::vec2>& _points) {
    glm::vec2 centroid;
    for (size_t i = 0; i < _points.size(); i++) {
        centroid += _points[i] / (float)_points.size();
    }
    return centroid;
}

glm::vec3 getCentroid(const std::vector<glm::vec3>& _points) {
    glm::vec3 centroid;
    for (size_t i = 0; i < _points.size(); i++) {
        centroid += _points[i] / (float)_points.size();
    }
    return centroid;
}

BoundingBox getBoundingBox(const Mesh& _mesh) {
    return getBoundingBox(_mesh.vertices);
}

BoundingBox getBoundingBox(const std::vector<glm::vec2>& _points ) {
    BoundingBox bbox;

    for (std::vector<glm::vec2>::const_iterator it = _points.begin(); it != _points.end(); ++it)
        bbox.expand(*it);
    
    return bbox;
}

BoundingBox getBoundingBox(const std::vector<glm::vec3>& _points ) {
    BoundingBox bbox;

    for (std::vector<glm::vec3>::const_iterator it = _points.begin(); it != _points.end(); ++it)
        bbox.expand(*it);
    
    return bbox;
}



// void computeSmoothingNormals(const Mesh& _mesh) {
//     std::vector<glm::vec3> smoothVertexNormals;

//     std::map<int, glm::vec3>::iterator iter;
//     for (size_t f = 0; f < _shape.mesh.indices.size() / 3; f++) {
//         // Get the three indexes of the face (all faces are triangular)
//         tinyobj::index_t idx0 = _shape.mesh.indices[3 * f + 0];
//         tinyobj::index_t idx1 = _shape.mesh.indices[3 * f + 1];
//         tinyobj::index_t idx2 = _shape.mesh.indices[3 * f + 2];

//         // Get the three vertex indexes and coordinates
//         int vi[3];      // indexes
//         vi[0] = idx0.vertex_index;
//         vi[1] = idx1.vertex_index;
//         vi[2] = idx2.vertex_index;

//         glm::vec3 v[3];  // coordinates
//         for (size_t i = 0; i < 3; i++)
//             v[i] = getVertex(_attrib, vi[i]);

//         // Compute the normal of the face
//         glm::vec3 normal;
//         calcNormal(v[0], v[1], v[2], normal);

//         // Add the normal to the three vertexes
//         for (size_t i = 0; i < 3; ++i) {
//             iter = smoothVertexNormals.find(vi[i]);
//             // add
//             if (iter != smoothVertexNormals.end())
//                 iter->second += normal;
//             else
//                 smoothVertexNormals[vi[i]] = normal;
//         }
//     }  // f

//     // Normalize the normals, that is, make them unit vectors
//     for (iter = smoothVertexNormals.begin(); iter != smoothVertexNormals.end(); iter++) {
//         iter->second = glm::normalize(iter->second);
//     }
// }


Mesh getSmoothNormals(const Mesh& _mesh, float _angle) {
    Mesh rta;

    std::vector<Triangle> triangles = _mesh.getTriangles();
    std::vector<glm::vec3> verts;
    for (size_t i = 0; i < triangles.size(); i++)
        for (size_t j = 0; j < 3; j++) 
            verts.push_back( triangles[i][j] );

    std::map<int, int> removeIds;
    float epsilon = .01f;
    for (size_t i = 0; i < verts.size()-1; i++) {
        for (size_t j = i+1; j < verts.size(); j++) {
            if (i != j) {
                const glm::vec3& v1 = verts[i];
                const glm::vec3& v2 = verts[j];
                if ( glm::distance(v1, v2) <= epsilon ) {
                    // average the location //
                    verts[i] = (v1+v2)/2.f;
                    verts[j] = verts[i];
                    removeIds[j] = 1;
                }
            }
        }
    }

    std::map<std::string, std::vector<int> > vertHash;
    std::string xStr, yStr, zStr;

    for (size_t i = 0; i < verts.size(); i++ ) {
        xStr = "x" + toString(verts[i].x == -0 ? 0: verts[i].x);
        yStr = "y" + toString(verts[i].y == -0 ? 0: verts[i].y);
        zStr = "z" + toString(verts[i].z == -0 ? 0: verts[i].z);
        std::string vstring = xStr+yStr+zStr;

        if (vertHash.find(vstring) == vertHash.end())
            for (size_t j = 0; j < triangles.size(); j++) 
                for (size_t k = 0; k < 3; k++) 
                    if (verts[i].x == triangles[j][k].x)
                        if (verts[i].y == triangles[j][k].y) 
                            if (verts[i].z == triangles[j][k].z)
                                vertHash[vstring].push_back( j );
    }

    glm::vec3 vert;
    glm::vec3 normal;
    float angleCos = cos(glm::radians(_angle));
    float numNormals = 0.0f;

    for (size_t j = 0; j < triangles.size(); j++) {
        for (size_t k = 0; k < 3; k++) {
            vert = triangles[j][k];
            xStr = "x" + toString(vert.x==-0?0:vert.x);
            yStr = "y" + toString(vert.y==-0?0:vert.y);
            zStr = "z" + toString(vert.z==-0?0:vert.z);

            std::string vstring = xStr+yStr+zStr;
            numNormals=0;
            normal = {0.f,0.f,0.f};
            if (vertHash.find(vstring) != vertHash.end()) {
                for (size_t i = 0; i < vertHash[vstring].size(); i++) {
                    glm::vec3 f1 = triangles[j].getNormal();
                    glm::vec3 f2 = triangles[vertHash[vstring][i]].getNormal();
                    if (glm::dot(f1, f2) >= angleCos ) {
                        normal += f2;
                        numNormals+=1.f;
                    }
                }
                //normal /= (float)vertHash[vstring].size();
                normal /= numNormals;

                triangles[j].setNormal(k, normal);
            }
        }
    }

    rta.addTriangles( triangles.data(), triangles.size() );

    return rta;
}

}

