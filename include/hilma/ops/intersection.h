#pragma once

#include "hilma/types/Ray.h"
#include "hilma/types/Line.h"
#include "hilma/types/Plane.h"
#include "hilma/types/Triangle.h"

#include <string>

// Based on
// https://github.com/robandrose/ofxIntersection
// http://paulbourke.net/geometry/pointlineplane/

namespace hilma {

// 2D
//
bool inside(const std::vector<glm::vec2> _points, const glm::vec2 _v);

bool intersection(  const glm::vec2 &_line1Start, const glm::vec2 &_line1End,
                    const glm::vec2 &_line2Start, const glm::vec2 &_line2End,
                    glm::vec2 &_intersection );


// 3D
//
struct IntersectionData {
    glm::vec3   pos;
    glm::vec3   dir;
    glm::vec3   normal;
    
    float       dist;
    bool        isIntersection;

    IntersectionData();
    std::string toString();
};

// Ray
IntersectionData intersection(const Ray& _ray, const Plane& _plane);
IntersectionData intersection(const Ray& _ray, const Triangle& _triangle);

// Line
IntersectionData intersection(const Line& _line, const Plane& _plane);
IntersectionData intersection(const Line& _line1, const Line& _line2);
IntersectionData intersection(const glm::vec3& _point, const Line& _line);

// Plane
IntersectionData intersection(const Plane& _plane1, const Plane& _plane2);
IntersectionData intersection(const Plane& _plane, const Triangle& _triangle);

}
