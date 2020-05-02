#pragma once

#include "hilma/Mesh.h"

namespace hilma {

// Mesh
//
void scale(Mesh& _mesh, float _v);
void scaleX(Mesh& _mesh, float _x);
void scaleY(Mesh& _mesh, float _y);
void scaleZ(Mesh& _mesh, float _z);
void scale(Mesh& _mesh, const glm::vec3& _v);
void scale(Mesh& _mesh, float _x, float _y, float _z);

void translateX(Mesh& _mesh, float _x);
void translateY(Mesh& _mesh, float _y);
void translateZ(Mesh& _mesh, float _z);
void translate(Mesh& _mesh, const glm::vec3& _v);
void translate(Mesh& _mesh, float _x, float _y, float _z);

// void rotateX(Mesh& _mesh, float _rad);
// void rotateY(Mesh& _mesh, float _rad);
// void rotateZ(Mesh& _mesh, float _rad);
// void rotate(Mesh& _mesh, float _rad, float _x, float _y, float _z );

void center(Mesh& _mesh);

// Polyline
// Mesh extrude(const std::vector<glm::vec3>& _polyline, float _height );
// Mesh extand(const std::vector<glm::vec3>& _polyline, float _height );

}