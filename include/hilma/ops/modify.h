#pragma once

#include "hilma/types/Mesh.h"

namespace hilma {

Mesh surface(const std::vector<glm::vec2>& _contour);
Mesh surface(const std::vector<std::vector<glm::vec2>>& _polygon);

Mesh extrude(const std::vector<glm::vec2>& _contour, float _maxHeight, float _minHeight = 0.0);
Mesh extrude(const std::vector<std::vector<glm::vec2>>& _polygon, float _maxHeight, float _minHeight = 0.0f);

enum class CapType {
    BUTT = 0,   // No points added to end of line
    SQUARE = 2, // Two points added to make a square extension
    ROUND = 6   // Six points added in a fan to make a round cap
};

enum class JoinType {
    MITER = 0,  // No points added at line join
    BEVEL = 1,  // One point added to flatten the corner of a join
    ROUND = 5   // Five points added in a fan to make a round outer join
};

Mesh spline(const std::vector<glm::vec2>& _polyline, float _width, JoinType _join = JoinType::MITER, CapType _cap = CapType::BUTT, float _miterLimit = 3.0);// , bool _close = false);

}