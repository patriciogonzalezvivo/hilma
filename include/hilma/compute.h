#pragma once

#include "hilma/Mesh.h"
#include "hilma/BoundingBox.h"

namespace hilma {

// 2D
bool                    isInside(const std::vector<glm::vec2> _points, const glm::vec2 _v);
void                    simplify(std::vector<glm::vec2>& _points, float _tolerance=0.3f);
std::vector<glm::vec2>  getSimplify(const std::vector<glm::vec2>& _points, float _tolerance=0.3f);
std::vector<glm::vec2>  getConvexHull(const std::vector<glm::vec2>& _points);
float                   getArea(const std::vector<glm::vec2>& _points);

// 2D & 3D
BoundingBox             getBoundingBox(const Mesh& _mesh);
BoundingBox             getBoundingBox(const std::vector<glm::vec2>& _points);
BoundingBox             getBoundingBox(const std::vector<glm::vec3>& _points);

glm::vec2               getCentroid(const std::vector<glm::vec2>& _points);
glm::vec3               getCentroid(const std::vector<glm::vec3>& _points);

// // 3D

}