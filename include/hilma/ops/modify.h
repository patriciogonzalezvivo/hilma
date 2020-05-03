#pragma once

#include "hilma/types/Mesh.h"
#include "hilma/types/Polyline.h"

namespace hilma {

Mesh spline(const Polyline& _polyline, float _width, JoinType _join = JoinType::MITER, CapType _cap = CapType::BUTT, float _miterLimit = 3.0);
Mesh spline(const std::vector<glm::vec2>& _polyline, float _width, JoinType _join = JoinType::MITER, CapType _cap = CapType::BUTT, float _miterLimit = 3.0);// , bool _close = false);

Mesh surface(const Polyline& _polyline);
Mesh surface(const std::vector<glm::vec2>& _contour);
Mesh surface(const std::vector<std::vector<glm::vec2>>& _polygon);

Mesh extrude(const Polyline& _polyline, float _maxHeight, float _minHeight = 0.0);
Mesh extrude(const std::vector<glm::vec2>& _contour, float _maxHeight, float _minHeight = 0.0);
Mesh extrude(const std::vector<std::vector<glm::vec2>>& _polygon, float _maxHeight, float _minHeight = 0.0f);

}