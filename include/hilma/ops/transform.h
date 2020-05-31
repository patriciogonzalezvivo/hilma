#pragma once

#include "hilma/types/Mesh.h"
#include "hilma/types/Image.h"
#include "hilma/types/Polyline.h"

#include <glm/gtc/quaternion.hpp>

namespace hilma {

// points
void transform(std::vector<glm::vec3>& _points, const glm::quat& _mat);
void transform(std::vector<glm::vec3>& _points, const glm::mat3& _mat);
void transform(std::vector<glm::vec3>& _points, const glm::mat4& _mat);

void scale(std::vector<glm::vec3>& _points, float _v);
void scaleX(std::vector<glm::vec3>& _points, float _x);
void scaleY(std::vector<glm::vec3>& _points, float _y);
void scaleZ(std::vector<glm::vec3>& _points, float _z);
void scale(std::vector<glm::vec3>& _points, const glm::vec3& _v);
void scale(std::vector<glm::vec3>& _points, float _x, float _y, float _z);

void translateX(std::vector<glm::vec3>& _points, float _x);
void translateY(std::vector<glm::vec3>& _points, float _y);
void translateZ(std::vector<glm::vec3>& _points, float _z);
void translate(std::vector<glm::vec3>& _points, const glm::vec3& _v);
void translate(std::vector<glm::vec3>& _points, float _x, float _y, float _z);

void translateY(std::vector<glm::vec3>& _points, const Image& _grayscale);
void translateZ(std::vector<glm::vec3>& _points, const Image& _grayscale);

void rotateX(std::vector<glm::vec3>& _points, float _rad);
void rotateY(std::vector<glm::vec3>& _points, float _rad);
void rotateZ(std::vector<glm::vec3>& _points, float _rad);
void rotate(std::vector<glm::vec3>& _points, float _rad, const glm::vec3& _axis );
void rotate(std::vector<glm::vec3>& _points, float _rad, float _x, float _y, float _z );

void center(std::vector<glm::vec3>& _points);

// Polyline
//
inline void scale(Polyline& _polyline, float _v) { scale(_polyline.points, _v); };
inline void scaleX(Polyline& _polyline, float _x) { scaleX(_polyline.points, _x); };
inline void scaleY(Polyline& _polyline, float _y) { scaleY(_polyline.points, _y); };
inline void scaleZ(Polyline& _polyline, float _z) { scaleZ(_polyline.points, _z); };
inline void scale(Polyline& _polyline, const glm::vec3& _v) { scale(_polyline.points, _v); };
inline void scale(Polyline& _polyline, float _x, float _y, float _z) { scale(_polyline.points, _x, _y, _z); };

inline void translateX(Polyline& _polyline, float _x) { translateX(_polyline.points, _x); };
inline void translateY(Polyline& _polyline, float _y) { translateY(_polyline.points, _y); };
inline void translateZ(Polyline& _polyline, float _z) { translateZ(_polyline.points, _z); };
inline void translate(Polyline& _polyline, const glm::vec3& _v) { translate(_polyline.points, _v); };
inline void translate(Polyline& _polyline, float _x, float _y, float _z) { translate(_polyline.points, _x, _y, _z);};

inline void translateY(Polyline& _polyline, const Image& _grayscale) { translateY(_polyline.points, _grayscale); };
inline void translateZ(Polyline& _polyline, const Image& _grayscale) { translateZ(_polyline.points, _grayscale); };

void rotateX(Polyline& _polyline, float _rad);
void rotateY(Polyline& _polyline, float _rad);
void rotateZ(Polyline& _polyline, float _rad);
void rotate(Polyline& _polyline, float _rad, const glm::vec3& _axis );
void rotate(Polyline& _polyline, float _rad, float _x, float _y, float _z );

inline void center(Polyline& _polyline) { center(_polyline.points); };

// Mesh
//
inline void scale(Mesh& _mesh, float _v) { scale(_mesh.vertices, _v); };
inline void scaleX(Mesh& _mesh, float _x) { scaleX(_mesh.vertices, _x); };
inline void scaleY(Mesh& _mesh, float _y) { scaleY(_mesh.vertices, _y); };
inline void scaleZ(Mesh& _mesh, float _z) { scaleZ(_mesh.vertices, _z); };
inline void scale(Mesh& _mesh, const glm::vec3& _v) { scale(_mesh.vertices, _v); };
inline void scale(Mesh& _mesh, float _x, float _y, float _z) { scale(_mesh.vertices, _x, _y, _z); };

inline void translateX(Mesh& _mesh, float _x) { translateX(_mesh.vertices, _x); };
inline void translateY(Mesh& _mesh, float _y) { translateY(_mesh.vertices, _y); };
inline void translateZ(Mesh& _mesh, float _z) { translateZ(_mesh.vertices, _z); };
inline void translate(Mesh& _mesh, const glm::vec3& _v) { translate(_mesh.vertices, _v); };
inline void translate(Mesh& _mesh, float _x, float _y, float _z) { translate(_mesh.vertices, _x, _y, _z);};

inline void translateY(Mesh& _mesh, const Image& _grayscale) { translateY(_mesh.vertices, _grayscale); };
inline void translateZ(Mesh& _mesh, const Image& _grayscale) { translateZ(_mesh.vertices, _grayscale); };

void rotateX(Mesh& _mesh, float _rad);
void rotateY(Mesh& _mesh, float _rad);
void rotateZ(Mesh& _mesh, float _rad);
void rotate(Mesh& _mesh, float _rad, const glm::vec3& _axis );
void rotate(Mesh& _mesh, float _rad, float _x, float _y, float _z );

inline void center(Mesh& _mesh) { center(_mesh.vertices); };

}