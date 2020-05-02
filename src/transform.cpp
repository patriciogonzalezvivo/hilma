#include "hilma/transform.h"
#include "hilma/compute.h"

namespace hilma {

void scale(Mesh& _mesh, float _v){
    for(std::vector<glm::vec3>::iterator it = _mesh.m_vertices.begin(); it != _mesh.m_vertices.end(); ++it)
        *it *= _v;
}

void scaleX(Mesh& _mesh, float _x){
    for(std::vector<glm::vec3>::iterator it = _mesh.m_vertices.begin(); it != _mesh.m_vertices.end(); ++it)
        it->x *= _x;
}

void scaleY(Mesh& _mesh, float _y){
    for(std::vector<glm::vec3>::iterator it = _mesh.m_vertices.begin(); it != _mesh.m_vertices.end(); ++it)
        it->y *= _y;
}

void scaleZ(Mesh& _mesh, float _z){
    for(std::vector<glm::vec3>::iterator it = _mesh.m_vertices.begin(); it != _mesh.m_vertices.end(); ++it)
        it->z *= _z;
}

void scale(Mesh& _mesh, float _x, float _y, float _z){
    scale(_mesh, glm::vec3(_x, _y, _z));
}

void scale(Mesh& _mesh, const glm::vec3& _v ){
    for(std::vector<glm::vec3>::iterator it = _mesh.m_vertices.begin(); it != _mesh.m_vertices.end(); ++it)
        *it *= _v;
}


void translateX(Mesh& _mesh, float _x){
    for(std::vector<glm::vec3>::iterator it = _mesh.m_vertices.begin(); it != _mesh.m_vertices.end(); ++it)
        it->x += _x;
}

void translateY(Mesh& _mesh, float _y){
    for(std::vector<glm::vec3>::iterator it = _mesh.m_vertices.begin(); it != _mesh.m_vertices.end(); ++it)
        it->y += _y;
}

void translateZ(Mesh& _mesh, float _z){
    for(std::vector<glm::vec3>::iterator it = _mesh.m_vertices.begin(); it != _mesh.m_vertices.end(); ++it)
        it->z += _z;
}

void translate(Mesh& _mesh, float _x, float _y, float _z){
    translate(_mesh, glm::vec3(_x, _y, _z));
}

void translate(Mesh& _mesh, const glm::vec3& _v) {
    for(std::vector<glm::vec3>::iterator it = _mesh.m_vertices.begin(); it != _mesh.m_vertices.end(); ++it)
        *it += _v;
}

// void rotateX(Mesh& _mesh, float _rad){

// }

// void rotateY(Mesh& _mesh, float _rad){

// }

// void rotateZ(Mesh& _mesh, float _rad){

// }

// void rotate(Mesh& _mesh, float _rad, float _x, float _y, float _z ) {

// }


void center(Mesh& _mesh){
    BoundingBox bbox = boundingBox(_mesh.m_vertices);
    glm::vec3 center = bbox.getCenter();
    translate(_mesh, -center);
}

}
