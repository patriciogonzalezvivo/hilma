#pragma once

#include "hilma/Mesh.h"
#include "hilma/BoundingBox.h"

namespace hilma {

BoundingBox              boundingBox(const std::vector<glm::vec3>& _points);
// glm::vec3                centroid(const std::vector<glm::vec3>& _points);
// float                    area(const std::vector<glm::vec3>& _points);

// std::vector<glm::vec3>   simplify(const std::vector<glm::vec3>& _points, float _tolerance=0.3f);
// std::vector<glm::vec3>   convexHul(const std::vector<glm::vec3>& _points, float _tolerance=0.3f);

}