#include "hilma/types/triangle.h"


#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"
#include "glm/glm.hpp"

using namespace hilma;


Triangle::Triangle() : m_colors(false), m_tangents(false), m_normals(false), m_texcoords(false) { 

}

Triangle::Triangle(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2) : m_colors(false), m_tangents(false), m_normals(false), m_texcoords(false) {
    set(_p0,_p1, _p2);
}

Triangle::~Triangle() {
}

void Triangle::set(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2) {
    m_vert_positions[0] = _p0;
    m_vert_positions[1] = _p1;
    m_vert_positions[2] = _p2;
    m_centroid = (m_vert_positions[0] + m_vert_positions[1] + m_vert_positions[2]) * 0.3333333333333f;
    m_normal = glm::cross(m_vert_positions[0] - m_vert_positions[2], m_vert_positions[1] - m_vert_positions[0]);
    m_area = glm::length( m_normal );
    m_normal = m_normal / m_area;
    // normal = glm::normalize( normal );
}

glm::vec3 Triangle::getBarycentricOf(const glm::vec3& _p) const {
    const glm::vec3 f0 = m_vert_positions[0] - _p;
    const glm::vec3 f1 = m_vert_positions[1] - _p;
    const glm::vec3 f2 = m_vert_positions[2] - _p;
    // calculate the areas and factors (order of parameters doesn't matter):
    // float a = glm::length(glm::cross(m_vert_positions[0] - m_vert_positions[1], m_vert_positions[0] - m_vert_positions[2])); // main triangle area a
    return glm::vec3(   glm::length(glm::cross(f1, f2)),        // p1's triangle area / a
                        glm::length(glm::cross(f2, f0)),        // p2's triangle area / a 
                        glm::length(glm::cross(f0, f1))) / m_area;   // p3's triangle area / a
}

bool Triangle::containsPoint(const glm::vec3 &_p){
    const glm::vec3 v0 = m_vert_positions[2] - m_vert_positions[0];
    const glm::vec3 v1 = m_vert_positions[1] - m_vert_positions[0];
    const glm::vec3 v2 = _p - m_vert_positions[0];
    
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

void Triangle::setVertex(size_t _index, const glm::vec3& _vertex) {
    m_vert_positions[_index] = _vertex;
}

void Triangle::setColor(const glm::vec4 &_color) {
    m_vert_colors[0] = _color;
    m_vert_colors[1] = _color;
    m_vert_colors[2] = _color; 
    m_colors = true;
}

void Triangle::setColor(float _r, float _g, float _b, float _a) {
    setColor(glm::vec4(_r, _g, _b, _a));
}

void Triangle::setColor(size_t _index, const glm::vec4& _color) {
    if (!m_colors)
        setColor(_color);
    m_vert_colors[_index] = _color;
}

void Triangle::setNormal(size_t _index, const glm::vec3& _normal) {
    if (!m_normals) {
        m_vert_normals[0] = _normal;
        m_vert_normals[1] = _normal;
        m_vert_normals[2] = _normal;
        m_normals = true;
    }
    m_vert_normals[_index] = _normal;
}

void Triangle::setTexCoord(size_t _index, const glm::vec2& _texcoord) {
    if (!m_texcoords) {
        m_vert_texcoords[0] = _texcoord;
        m_vert_texcoords[1] = _texcoord;
        m_vert_texcoords[2] = _texcoord;
        m_texcoords = true;
    }
    m_vert_texcoords[_index] = _texcoord;
}

void Triangle::setTangent(size_t _index, const glm::vec4& _tangent) {
    if (!m_tangents) {
        m_vert_tangents[0] = _tangent;
        m_vert_tangents[1] = _tangent;
        m_vert_tangents[2] = _tangent;
        m_tangents = true;
    }
    m_vert_tangents[_index] = _tangent;
}

void Triangle::setColors(const glm::vec4 &_p0, const glm::vec4 &_p1, const glm::vec4 &_p2) {
    m_vert_colors[0] = _p0;
    m_vert_colors[1] = _p1;
    m_vert_colors[2] = _p2; 
    m_colors = true;
}

void Triangle::setNormals(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2) {
    m_vert_normals[0] = _p0;
    m_vert_normals[1] = _p1;
    m_vert_normals[2] = _p2; 
    m_normals = true;
}

void Triangle::setTexCoords(const glm::vec2 &_p0, const glm::vec2 &_p1, const glm::vec2 &_p2) {
    m_vert_texcoords[0] = _p0;
    m_vert_texcoords[1] = _p1;
    m_vert_texcoords[2] = _p2; 
    m_texcoords = true;
}

void Triangle::setTangents(const glm::vec4 &_p0, const glm::vec4 &_p1, const glm::vec4 &_p2) {
    m_vert_tangents[0] = _p0;
    m_vert_tangents[1] = _p1;
    m_vert_tangents[2] = _p2; 
    m_tangents = true;
}

glm::vec3 Triangle::getVertex(const glm::vec3& _barycenter) const {
    return  getVertex(0) * _barycenter.x +
            getVertex(1) * _barycenter.y +
            getVertex(2) * _barycenter.z;
}

glm::vec4 Triangle::getColor(const glm::vec3& _barycenter) const {
    // if (material != nullptr) {
    //     if ( material->haveProperty("diffuse") ) {
    //         if (haveTexCoords()) {
    //             glm::vec2 uv = getTexCoord(_barycenter);
    //             return material->getColor("diffuse", uv);
    //         }
    //         else
    //             return material->getColor("diffuse");
    //     }
    // }

    if (haveColors())
        return  getColor(0) * _barycenter.x +
                getColor(1) * _barycenter.y +
                getColor(2) * _barycenter.z;
    else
        return glm::vec4(1.0f);
}

glm::vec3 Triangle::getNormal(const glm::vec3& _barycenter) const {
    glm::vec3 n;
    if (haveNormals())
        n = getNormal(0) * _barycenter.x +
            getNormal(1) * _barycenter.y +
            getNormal(2) * _barycenter.z;
    else
        n = getNormal();

    // if (material != nullptr && haveTexCoords() && haveTangents()) {
    //     if ( material->haveProperty("normalmap") ) {
    //         glm::vec2 uv = getTexCoord(_barycenter);
    //         glm::vec4 t = getTangent(_barycenter);
    //         glm::vec3 b = glm::cross( n, glm::vec3(t.x, t.y, t.z) ) * t.w;
    //         glm::mat3 tbn = glm::mat3( t, b, n );

    //         return tbn * ( material->getColor("normalmap", uv) * 2.0f - 1.0f);
    //     }
    // }

    return n;
}

glm::vec2 Triangle::getTexCoord(const glm::vec3& _barycenter) const {
    glm::vec2 uv =  getTexCoord(0) * _barycenter.x +
                    getTexCoord(1) * _barycenter.y +
                    getTexCoord(2) * _barycenter.z;
    
    uv.x = 1.0 - uv.x;

    return uv;
}

glm::vec4 Triangle::getTangent(const glm::vec3& _barycenter ) const {
    return  getTangent(0) * _barycenter.x +
            getTangent(1) * _barycenter.y +
            getTangent(2) * _barycenter.z;
}

