#include "hilma/ops/intersection.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"

#include "hilma/math.h"
#include "hilma/text.h"

#ifndef ABS
#define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

namespace hilma {

static uint64_t numRayBoxTests = 0; 
static uint64_t numRayTrianglesTests = 0; 
static uint64_t numRayTrianglesIsect = 0; 

const float infinity = std::numeric_limits<float>::infinity();

// http://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
//
bool inside(const std::vector<glm::vec2> _points, const glm::vec2 _v) {
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

bool intersection(  const glm::vec2 &_line1Start, const glm::vec2 &_line1End,
                    const glm::vec2 &_line2Start, const glm::vec2 &_line2End,
                    glm::vec2 &_intersection ) {
    glm::vec2 diffLA, diffLB;
    double compareA, compareB;
    diffLA = _line1End - _line1Start;
    diffLB = _line2End - _line2Start;
    compareA = diffLA.x*_line1Start.y - diffLA.y*_line1Start.x;
    compareB = diffLB.x*_line2Start.y - diffLB.y*_line2Start.x;
    if (
        (
         ( ( diffLA.x*_line2Start.y - diffLA.y*_line2Start.x ) < compareA ) ^
         ( ( diffLA.x*_line2End.y - diffLA.y*_line2End.x ) < compareA )
         )
        &&
        (
         ( ( diffLB.x*_line1Start.y - diffLB.y*_line1Start.x ) < compareB ) ^
         ( ( diffLB.x*_line1End.y - diffLB.y*_line1End.x) < compareB )
         )
        )
    {
        double lDetDivInv = 1 / ((diffLA.x*diffLB.y) - (diffLA.y*diffLB.x));
        _intersection.x =  -((diffLA.x*compareB) - (compareA*diffLB.x)) * lDetDivInv ;
        _intersection.y =  -((diffLA.y*compareB) - (compareA*diffLB.y)) * lDetDivInv ;
        
        return true;
    }
    
    return false;
};



/************** Ray ************/
IntersectionData intersection(const Ray& _ray, const Plane& _plane) {
    IntersectionData idata;
    
    float u = 0.0f;
    float denom = glm::dot(_plane.getNormal(), _ray.getDirection());
    
    // check if _ray is paralles to _plane:
    idata.hit = fabs(denom) > EPS;
    if (idata.hit) {
        u = glm::dot(_plane.getNormal(), _plane.getOrigin() - _ray.getOrigin()) / denom;
        idata.position = _ray.getOrigin() + _ray.getDirection() * u;
    }

    return idata;
}

bool intersection(const Ray& _ray, const BoundingBox& _bbox, float &_tmin, float &_tmax) {
    __sync_fetch_and_add(&numRayBoxTests, 1); 

    // float tx1 = (_bbox.min.x - _ray.getOrigin().x)*_ray.getInvertDirection().x;
    // float tx2 = (_bbox.max.x - _ray.getOrigin().x)*_ray.getInvertDirection().x;
 
    // _tmin = std::min(tx1, tx2);
    // _tmax = std::max(tx1, tx2);
 
    // float ty1 = (_bbox.min.y - _ray.getOrigin().y)*_ray.getInvertDirection().y;
    // float ty2 = (_bbox.max.y - _ray.getOrigin().y)*_ray.getInvertDirection().y;
 
    // _tmin = std::max(_tmin, std::min(ty1, ty2));
    // _tmax = std::min(_tmax, std::max(ty1, ty2));

    // float tz1 = (_bbox.min.z - _ray.getOrigin().z)*_ray.getInvertDirection().z;
    // float tz2 = (_bbox.max.z - _ray.getOrigin().z)*_ray.getInvertDirection().z;
 
    // _tmin = std::max(_tmin, std::min(tz1, tz2));
    // _tmax = std::min(_tmax, std::max(tz1, tz2));
 
    // return _tmax >= _tmin;

    // Increase far parameter to avoid false positives due to rounding.
    static float error_padding = 1.0f + 2.0f * tgamma(3.0f);

    float t0 = 0.0f;
    float t1 = infinity;

    for (size_t i = 0; i < 3; ++i) {
        float near = (_bbox.min[i] - _ray.getOrigin()[i]) * _ray.getInvertDirection()[i];
        float far = (_bbox.max[i] - _ray.getOrigin()[i]) * _ray.getInvertDirection()[i];
        if (near > far) std::swap(near, far);
        far *= error_padding;
        t0 = near > t0 ? near : t0;
        t1 = far < t1 ? far : t1;
        if (t0 > t1) return false;
    }

    _tmin = t0;
    _tmax = t1;

    return true;
}

IntersectionData intersection(const Ray& _ray, const BoundingBox& _bbox) {
    IntersectionData idata;

    float tmin, tmax;
    idata.hit = intersection(_ray, _bbox, tmin, tmax);

    if (!idata.hit) return idata;

    idata.distance = tmin;
    idata.position = _ray.getAt(tmin);

    return idata;
}

// MOLLER_TRUMBORE
// #define CULLING
bool intersectionMT(const Ray& _ray, const Triangle& _triangle, float& _t, float& _u, float& _v) {

    __sync_fetch_and_add(&numRayTrianglesTests, 1); 

    glm::vec3 v0v1 = _triangle[1] - _triangle[0]; 
    glm::vec3 v0v2 = _triangle[2] - _triangle[0]; 
    glm::vec3 pvec = glm::cross(_ray.getDirection(), v0v2); 
    float det = glm::dot(v0v1, pvec); 
#ifdef CULLING 
    // if the determinant is negative the triangle is backfacing
    // if the determinant is close to 0, the ray misses the triangle
    if (det < EPS) return false; 
#else 
    // ray and triangle are parallel if det is close to 0
    if (fabs(det) < EPS) return false; 
#endif 
    float invDet = 1 / det; 
 
    glm::vec3 tvec = _ray.getOrigin() - _triangle[0]; 
    _u = glm::dot(tvec, pvec) * invDet; 
    if (_u < 0 || _u > 1) return false; 
 
    glm::vec3 qvec = glm::cross(tvec, v0v1); 
    _v = glm::dot(_ray.getDirection(), qvec) * invDet; 
    if (_v < 0 || _u + _v > 1) return false; 
 
    _t = glm::dot(v0v2, qvec) * invDet; 

    __sync_fetch_and_add(&numRayTrianglesIsect, 1); 
 
    return true; 
}

bool intersection(const Ray& _ray, const Triangle& _triangle, float& _t, float& _u, float& _v) {

    __sync_fetch_and_add(&numRayTrianglesTests, 1); 

    // no need to normalize
    glm::vec3 N = _triangle.getNormal(); 
    float denom = glm::dot(N, N); 
 
    // Step 1: finding P
 
    // check if ray and plane are parallel ?
    float NdotRayDirection = glm::dot(N, _ray.getDirection()); 
    if (fabs(NdotRayDirection) < EPS) // almost 0 
        return false; // they are parallel so they don't intersect ! 
 
    // compute d parameter using equation 2
    float d = glm::dot(N, _triangle[0]); 
 
    // compute t (equation 3)
    _t = (glm::dot(N, _ray.getOrigin()) + d) / NdotRayDirection; 
    // check if the triangle is in behind the ray
    if (_t < 0) return false; // the triangle is behind 
 
    // compute the intersection point using equation 1
    glm::vec3 P = _ray.getOrigin() + _t * _ray.getDirection(); 
 
    // Step 2: inside-outside test
    glm::vec3 C; // vector perpendicular to triangle's plane 
 
    // edge 0
    glm::vec3 edge0 = _triangle[1] - _triangle[0]; 
    glm::vec3 vp0 = P - _triangle[0]; 
    C = glm::cross(edge0, vp0); 
    if (glm::dot(N, C) < 0) return false; // P is on the right side 
 
    // edge 1
    glm::vec3 edge1 = _triangle[2] - _triangle[1]; 
    glm::vec3 vp1 = P - _triangle[1]; 
    C = glm::cross(edge1, vp1); 
    if ((_u = glm::dot(N, C)) < 0)  return false; // P is on the right side 
 
    // edge 2
    glm::vec3 edge2 = _triangle[0] - _triangle[2]; 
    glm::vec3 vp2 = P - _triangle[2]; 
    C = glm::cross(edge2, vp2); 
    if ((_v = glm::dot(N, C)) < 0) return false; // P is on the right side; 
 
    _u /= denom; 
    _v /= denom; 
 
    __sync_fetch_and_add(&numRayTrianglesIsect, 1); 

    return true; // this ray hits the triangle 
} 

uint64_t getTotalRayBoundingBoxTests() {
    return numRayBoxTests;
}
uint64_t getTotalRayTriangleTests() {
    return numRayTrianglesTests;
}

uint64_t getTotalRayTrianglesIntersections() {
    return numRayTrianglesIsect;
}

IntersectionData intersection(const Ray& _ray, const Triangle& _triangle) {

    IntersectionData idata;

    float t,u,v;
    idata.hit = intersection(_ray, _triangle, t, u, v);
    // idata.hit = intersectionMT(_ray, _triangle, t, u, v);

    if (!idata.hit) return idata;

    // float vn = glm::dot(_ray.getDirection(), _triangle.getNormal());
    // glm::vec3 aa = _ray.getOrigin() - _triangle[0];
    // float xpn = glm::dot(aa, _triangle.getNormal());
    // idata.distance = -xpn / vn;
    idata.distance = t;
    idata.position = _ray.getAt(t);
    return idata;

}



float PointPlaneDistance(const glm::vec3& _point, const Plane& _plane) {
    return glm::dot(_plane.getNormal(), _point - _plane.getOrigin());
}

// /************** Line  ************/

IntersectionData intersection(const Line& _line, const Plane& _plane) {
    IntersectionData idata;
    
    float dist1 = PointPlaneDistance(_line[0], _plane);
    float dist2 = PointPlaneDistance(_line[1], _plane);
    int pos1 = signValue(dist1);
    int pos2 = signValue(dist2);
    
    if (pos1==pos2) {
        idata.hit=false;
        return idata;
    }
    
    float u = 0.0;
    float denom = glm::dot(_plane.getNormal(),_line.getVector());
    
    // check if ray is paralles to plane:
    
    if (fabs(denom) > EPS) {
        u= glm::dot(_plane.getNormal(),_plane.getOrigin()-_line[0])/denom;
        // check if intersection is within line-segment:
        if (u>1.0 || u<0) {
            idata.hit=false;
            return idata;
        }
        idata.hit=true;
        idata.position = _line[0]+_line.getVector()*u;
    }
    else
        idata.hit=false;
    

    return idata;
}

IntersectionData intersection(const Line& _line1, const Line& _line2) {
    IntersectionData idata;
    
    glm::vec3 p13,p43,p21;
    
    glm::vec3 p1 = _line1[0];
    glm::vec3 p2 = _line1[1];
    glm::vec3 p3 = _line2[0];
    glm::vec3 p4 = _line2[1];
    
    
    double d1343, d4321, d1321, d4343, d2121;
    double numer, denom;
    double mua, mub;
    glm::vec3 pa, pb;
    
    p13.x = p1.x - p3.x;
    p13.y = p1.y - p3.y;
    p13.z = p1.z - p3.z;
    p43.x = p4.x - p3.x;
    p43.y = p4.y - p3.y;
    p43.z = p4.z - p3.z;
    if (ABS(p43.x) < EPS && ABS(p43.y) < EPS && ABS(p43.z) < EPS) {
        idata.hit=false;
        return idata;
    }
    
    p21.x = p2.x - p1.x;
    p21.y = p2.y - p1.y;
    p21.z = p2.z - p1.z;
    
    if (ABS(p21.x) < EPS && ABS(p21.y) < EPS && ABS(p21.z) < EPS) {
        idata.hit=false;
        return idata;
    }
    
    d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
    d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
    d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
    d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
    d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;
    
    denom = d2121 * d4343 - d4321 * d4321;
    if (ABS(denom) < EPS) {
        idata.hit=false;
        return idata;
    }
    
    numer = d1343 * d4321 - d1321 * d4343;
    mua = numer / denom;
    mub = (d1343 + d4321 * (mua)) / d4343;
    
    pa.x = p1.x + mua * p21.x;
    pa.y = p1.y + mua * p21.y;
    pa.z = p1.z + mua * p21.z;
    pb.x = p3.x + mub * p43.x;
    pb.y = p3.y + mub * p43.y;
    pb.z = p3.z + mub * p43.z;
    
    idata.hit = true;
    idata.position = pa;
    idata.direction = pb-pa;
    return idata;
    
}


IntersectionData intersection(const glm::vec3& _point, const Line& _line) {
    float u;
    glm::vec3 dist;
    IntersectionData idata;
    
    u = ( ( ( _point.x - _line[0].x ) * ( _line.getVector().x ) +
        ( ( _point.y - _line[0].y ) * ( _line.getVector().y) ) +
        ( ( _point.z - _line[0].z ) * ( _line.getVector().z) ) ) /
        ( glm::length2( _line.getVector() ) ) );

    if ( u < 0.0f || u > 1.0f ) {
        idata.hit = false;
        return idata;
    }
    
    idata.hit = true;
    idata.position = _line[0] + u * _line.getVector();
    idata.direction = idata.position - _point;
    
    return idata;
}


// /************** Plane  ************/

IntersectionData intersection(const Plane& _plane1, const Plane& _plane2) {
    IntersectionData idata;
    
    glm::vec3 n1 = _plane1.getNormal();
    glm::vec3 n2 = _plane2.getNormal();
    float d1 = _plane1.getDCoeff();
    float d2 = _plane2.getDCoeff();
    
    // Check if planes are parallel, if so return false:
    glm::vec3 dir= glm::cross(_plane1.getNormal(),_plane2.getNormal());
    
    idata.hit = dir.length() < EPS;

    if (idata.hit) {
        // Direction of intersection is the cross product of the two normals:
        dir = glm::normalize(dir);
        
        // Thank you Toxi!
        float offDiagonal = glm::dot(n1,n2);
        double det = 1.0 / (1 - offDiagonal * offDiagonal);
        double a = (d1 - d2 * offDiagonal) * det;
        double b = (d2 - d1 * offDiagonal) * det;
        glm::vec3 anchor = getScaled(n1,(float)a) + getScaled(n2,(float)b);
        
        idata.position = anchor;
        idata.direction = dir;
    }
    
    return idata;
}


glm::vec3 LinePlaneIntersectionFast(const glm::vec3& _p0, const glm::vec3& _p1, const Plane& _plane) {
    glm::vec3 vec(_p1-_p0);
    float denom = glm::dot(_plane.getNormal(),vec);
    float u = glm::dot(_plane.getNormal(),_plane.getOrigin()-_p0)/denom;
    return _p0 + vec * u;
}

bool containsValue(std::vector<glm::vec3>* _points, const glm::vec3& _point) {
    for (int i = 0; i < _points->size(); i++)
        if ((_points->at(i)-_point).length()<EPS )
            return true;   
    return false;
}

IntersectionData intersection(const Plane& _plane, const Triangle& _triangle) {
    
    IntersectionData idata;
    glm::vec3 tp0 = _triangle[0];
    glm::vec3 tp1 = _triangle[1];
    glm::vec3 tp2 = _triangle[2];
    
    float dist1 = PointPlaneDistance(tp0, _plane);
    float dist2 = PointPlaneDistance(tp1, _plane);
    float dist3 = PointPlaneDistance(tp2, _plane);
    
    int pos1 = signValue(dist1);
    int pos2 = signValue(dist2);
    int pos3 = signValue(dist3);
    
    if (pos1==pos2 && pos1==pos3) {
        idata.hit=false;
        return idata;
    };
    
    std::vector<glm::vec3>ispoints;
    bool bintersects=false;
    glm::vec3 ip;
    
    if (pos1!=pos2) {
        ip = LinePlaneIntersectionFast(tp0, tp1, _plane);
        if (!containsValue(&ispoints, ip)) {
            ispoints.push_back(ip);
        };
    }
    if (pos2!=pos3) {
        ip = LinePlaneIntersectionFast(tp1, tp2, _plane);
        if (!containsValue(&ispoints, ip)) {
            ispoints.push_back(ip);
        };
    }
    if (pos3!=pos1) {
        ip = LinePlaneIntersectionFast(tp2, tp0, _plane);
        if (!containsValue(&ispoints, ip)) {
            ispoints.push_back(ip);
        };
    }
    
    idata.hit = true;
    idata.position = ispoints.at(0);
    
    if ( ispoints.size() == 2) {
        idata.direction = ispoints.at(1);
        idata.direction -= idata.position;
        idata.distance = idata.direction.length();
    }
    
    return idata;
}

}