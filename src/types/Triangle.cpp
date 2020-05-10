#include "hilma/types/Triangle.h"

using namespace hilma;


Triangle::Triangle() { 

}

Triangle::Triangle(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2) {
    set(_p0,_p1, _p2);
}

Triangle::~Triangle() {
}

void Triangle::set(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2) {
    vertices[0] = _p0;
    vertices[1] = _p1;
    vertices[2] = _p2;
    normal = glm::normalize( glm::cross(vertices[0] - vertices[2], vertices[1] - vertices[0]) );
}

void Triangle::setVertex(size_t _index, const glm::vec3& _vertex) {
    vertices[_index] = _vertex;
}

void Triangle::setColor(const glm::vec4 &_color) {
    if (colors.empty())
        colors.resize(3);

    std::fill(colors.begin(),colors.begin()+3, _color);
}

void Triangle::setColor(float _r, float _g, float _b, float _a) {
    setColor(glm::vec4(_r, _g, _b, _a));
}

void Triangle::setColor(size_t _index, const glm::vec4& _color) {
    if (colors.empty()) {
        colors.resize(3);
        std::fill(colors.begin(),colors.begin()+3, _color);
    }
    else
        colors[_index] = _color;
}

void Triangle::setNormal(size_t _index, const glm::vec3& _normal) {
        if (normals.empty()) {
        normals.resize(3);
        std::fill(normals.begin(),normals.begin()+3, _normal);
    }
    else
        normals[_index] = _normal;
}

void Triangle::setTexCoord(size_t _index, const glm::vec2& _texcoord) {
    if (texcoords.empty()) {
        texcoords.resize(3);
        std::fill(texcoords.begin(),texcoords.begin()+3, _texcoord);
    }
    else
        texcoords[_index] = _texcoord;
}

void Triangle::setColors(const glm::vec4 &_p0, const glm::vec4 &_p1, const glm::vec4 &_p2) {
    colors.resize(3);
    colors[0] = _p0;
    colors[1] = _p1;
    colors[2] = _p2;
}

void Triangle::setNormals(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2) {
    normals.resize(3);
    normals[0] = _p0;
    normals[1] = _p1;
    normals[2] = _p2;
}

void Triangle::setTexCoords(const glm::vec2 &_p0, const glm::vec2 &_p1, const glm::vec2 &_p2) {
    texcoords.resize(3);
    texcoords[0] = _p0;
    texcoords[1] = _p1;
    texcoords[2] = _p2;
}

bool Triangle::containsPoint(const glm::vec3 &_p){
    glm::vec3 v0 = vertices[2] - vertices[0];
    glm::vec3 v1 = vertices[1] - vertices[0];
    glm::vec3 v2 = _p - vertices[0];
    
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