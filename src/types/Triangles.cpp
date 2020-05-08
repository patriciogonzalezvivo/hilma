#include "hilma/types/Triangle.h"

using namespace hilma;


Triangle::Triangle(): colors(nullptr), normals(nullptr), texcoords(nullptr) { 

}

Triangle::Triangle(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2) {
    set(_p0,_p1, _p2);
}

Triangle::~Triangle() {
    // if (colors != nullptr) delete[] colors;
    // if (normals != nullptr) delete[] normals;
    // if (texcoords != nullptr) delete[] texcoords;
}

void Triangle::set(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2) {
    points[0] = _p0;
    points[1] = _p1;
    points[2] = _p2;
    normal = glm::normalize( glm::cross(points[0] - points[2], points[1] - points[0]) );
}

void Triangle::setColors(const glm::vec4 &_p0, const glm::vec4 &_p1, const glm::vec4 &_p2) {
    colors = new glm::vec4[3];
    colors[0] = _p0;
    colors[1] = _p1;
    colors[2] = _p2;
}

void Triangle::setNormals(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2) {
    normals = new glm::vec3[3];
    normals[0] = _p0;
    normals[1] = _p1;
    normals[2] = _p2;
}

void Triangle::setTexCoords(const glm::vec2 &_p0, const glm::vec2 &_p1, const glm::vec2 &_p2) {
    texcoords = new glm::vec2[3];
    texcoords[0] = _p0;
    texcoords[1] = _p1;
    texcoords[2] = _p2;
}

bool Triangle::containsPoint(const glm::vec3 &_p){
    glm::vec3 v0 = points[2] - points[0];
    glm::vec3 v1 = points[1] - points[0];
    glm::vec3 v2 = _p - points[0];
    
    // Compute dot products
    float dot00 = glm::dot(v0, v0);
    float dot01 = glm::dot(v0, v1);
    float dot02 = glm::dot(v0, v2);
    float dot11 = glm::dot(v1, v1);
    float dot12 = glm::dot(v1, v2);
    
    // Compute barycentric coordinates
    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    
    // Check if point is in triangle
    return (u >= 0.0) && (v >= 0.0) && (u + v <= 1.0);
}