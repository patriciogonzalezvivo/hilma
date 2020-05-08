#include "hilma/ops/compute.h"

#include <algorithm>

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

    if(_points.size() < 2) return;

    int n = _points.size();

    if(n == 0) {
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
    if( m < (int)sV.size() ) {
        _points.assign( sV.begin(),sV.begin()+m );
    }else{
        _points = sV;
    }
}

// http://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
//
bool isInside(const std::vector<glm::vec2> _points, const glm::vec2 _v) {
    int counter = 0;
    double xinters;
    glm::vec2 p1,p2;

    size_t N = _points.size();
    p1 = _points[0];
    for (size_t i = 1; i <= N; i++) {
        p2 = _points[i % N];
        if (_v.y > std::min(p1.y, p2.y)) {
            if (_v.y <= std::max(p1.y, p2.y)) {
                if (_v.x <= std::max(p1.x, p2.x)) {
                    if (p1.y != p2.y) {
                        xinters = (_v.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;
                        if (p1.x == p2.x || _v.x <= xinters)
                            counter++;
                    }
                }
            }
        }
        p1 = p2;
    }

    if (counter % 2 == 0) return false;
    else return true;
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
            if(currentPoint == 3 && direction == 1) {
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

    for(int i=0;i<(int)_points.size()-1;i++) {
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

    for(std::vector<glm::vec2>::const_iterator it = _points.begin(); it != _points.end(); ++it)
        bbox.expand(*it);
    
    return bbox;
}

BoundingBox getBoundingBox(const std::vector<glm::vec3>& _points ) {
    BoundingBox bbox;

    for(std::vector<glm::vec3>::const_iterator it = _points.begin(); it != _points.end(); ++it)
        bbox.expand(*it);
    
    return bbox;
}

}

