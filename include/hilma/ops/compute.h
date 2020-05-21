#pragma once

#include "hilma/types/Mesh.h"
#include "hilma/types/BoundingBox.h"
#include "hilma/types/Image.h"

namespace hilma {

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

// 3D
Mesh                    getSmoothNormals(const Mesh& _mesh, float _angle);

// Textures
Image                   getNormalMap(const Image& _heightmap, const float _zScale);

}