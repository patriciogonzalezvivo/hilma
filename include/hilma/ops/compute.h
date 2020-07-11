#pragma once

#include "hilma/types/Mesh.h"
#include "hilma/accel/BoundingBox.h"

namespace hilma {

// 2D
void                    simplify(std::vector<glm::vec2>& _points, float _tolerance=0.3f);
std::vector<glm::vec2>  getSimplify(const std::vector<glm::vec2>& _points, float _tolerance=0.3f);
std::vector<glm::vec2>  getConvexHull(const std::vector<glm::vec2>& _points);
float                   getArea(const std::vector<glm::vec2>& _points);

// 2D & 3D
BoundingBox             getBoundingBox(const Mesh& _mesh);
BoundingBox             getBoundingBox(const std::vector<glm::vec2>& _points);
BoundingBox             getBoundingBox(const std::vector<glm::vec3>& _points);
BoundingBox             getBoundingBox(const std::vector<Line>& _lines);
BoundingBox             getBoundingBox(const std::vector<Triangle>& _triangles);

glm::vec2               getCentroid(const std::vector<glm::vec2>& _points);
glm::vec3               getCentroid(const std::vector<glm::vec3>& _points);

std::vector<float>      getMax(const float* _array2D, int _m, int _n);
std::vector<float>      getMin(const float* _array2D, int _m, int _n);

// Image
glm::vec2               getRange(const Image& _image);

void                    textureAtlas(Mesh& _mesh);

}