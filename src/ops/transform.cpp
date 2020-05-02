#include "hilma/ops/transform.h"
#include "hilma/ops/compute.h"

#define GLM_ENABLE_EXPERIMENTAL 
#include <glm/gtx/quaternion.hpp>

namespace hilma {

// void scale(Mesh& _mesh, float _v) {
//     scale(_mesh.m_vertices, _v);
// }

// void scaleX(Mesh& _mesh, float _x) {
//     scaleX(_mesh.m_vertices, _x);
// }

// void scaleY(Mesh& _mesh, float _y) {
//     scaleY(_mesh.m_vertices, _y);
// }

// void scaleZ(Mesh& _mesh, float _z) {

// }



void scale(std::vector<glm::vec3>& _points, float _v){
    for (std::vector<glm::vec3>::iterator it = _points.begin(); it != _points.end(); ++it)
        *it *= _v;
}

void scaleX(std::vector<glm::vec3>& _points, float _x){
    for (std::vector<glm::vec3>::iterator it = _points.begin(); it != _points.end(); ++it)
        it->x *= _x;
}

void scaleY(std::vector<glm::vec3>& _points, float _y){
    for (std::vector<glm::vec3>::iterator it = _points.begin(); it != _points.end(); ++it)
        it->y *= _y;
}

void scaleZ(std::vector<glm::vec3>& _points, float _z){
    for (std::vector<glm::vec3>::iterator it = _points.begin(); it != _points.end(); ++it)
        it->z *= _z;
}

void scale(std::vector<glm::vec3>& _points, float _x, float _y, float _z){
    scale(_points, glm::vec3(_x, _y, _z));
}

void scale(std::vector<glm::vec3>& _points, const glm::vec3& _v ){
    for (std::vector<glm::vec3>::iterator it = _points.begin(); it != _points.end(); ++it)
        *it *= _v;
}


void translateX(std::vector<glm::vec3>& _points, float _x){
    for (std::vector<glm::vec3>::iterator it = _points.begin(); it != _points.end(); ++it)
        it->x += _x;
}

void translateY(std::vector<glm::vec3>& _points, float _y){
    for (std::vector<glm::vec3>::iterator it = _points.begin(); it != _points.end(); ++it)
        it->y += _y;
}

void translateZ(std::vector<glm::vec3>& _points, float _z){
    for (std::vector<glm::vec3>::iterator it = _points.begin(); it != _points.end(); ++it)
        it->z += _z;
}

void translate(std::vector<glm::vec3>& _points, float _x, float _y, float _z){
    translate(_points, glm::vec3(_x, _y, _z));
}

void translate(std::vector<glm::vec3>& _points, const glm::vec3& _v) {
    for (std::vector<glm::vec3>::iterator it = _points.begin(); it != _points.end(); ++it)
        *it += _v;
}

void rotate(std::vector<glm::vec3>& _points, float _rad, const glm::vec3& _axis ) {
    glm::quat q = glm::angleAxis(_rad, _axis);
    glm::mat3 M = glm::mat3_cast(q);
    for (std::vector<glm::vec3>::iterator it = _points.begin(); it != _points.end(); ++it)
        *it = M * *it;
}

void rotate(std::vector<glm::vec3>& _points, float _rad, float _x, float _y, float _z ) {
    rotate( _points, _rad, glm::vec3(_x, _y, _z));
}

void rotateX(std::vector<glm::vec3>& _points, float _rad){
    rotate( _points, _rad, 1.0f, 0.0f, 0.0f);
}

void rotateY(std::vector<glm::vec3>& _points, float _rad){
    rotate( _points, _rad, 0.0f, 1.0f, 0.0f);
}

void rotateZ(std::vector<glm::vec3>& _points, float _rad){
    rotate( _points, _rad, 0.0f, 0.0f, 1.0f);
}

void center(std::vector<glm::vec3>& _points){
    BoundingBox bbox = getBoundingBox(_points);
    glm::vec3 center = bbox.getCenter();
    translate(_points, -center);
}

void rotateX(Mesh& _mesh, float _rad) {
    rotateX(_mesh.m_vertices, _rad);
    if (_mesh.hasNormals()) rotateX(_mesh.m_normals, _rad);
}

void rotateY(Mesh& _mesh, float _rad) {
    rotateY(_mesh.m_vertices, _rad);
    if (_mesh.hasNormals()) rotateY(_mesh.m_normals, _rad);
}

void rotateZ(Mesh& _mesh, float _rad) {
    rotateZ(_mesh.m_vertices, _rad);
    if (_mesh.hasNormals()) rotateZ(_mesh.m_normals, _rad);
}

void rotate(Mesh& _mesh, float _rad, const glm::vec3& _axis ) {
    rotate(_mesh.m_vertices, _rad, _axis);
    if (_mesh.hasNormals()) rotate(_mesh.m_normals, _rad, _axis);
}

void rotate(Mesh& _mesh, float _rad, float _x, float _y, float _z ) {
    rotate(_mesh.m_vertices, _rad, _x, _y, _z);
    if (_mesh.hasNormals()) rotate(_mesh.m_normals, _rad, _x, _y, _z);
}


}
