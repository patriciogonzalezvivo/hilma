#pragma once 

#include <memory>
#include "glm/glm.hpp"

namespace hilma {

class Line{
public:
    Line() : direction(0.0) { };
    Line(const glm::vec3& _p0, const glm::vec3& _p1) {
        set(_p0,_p1);
    }
    
    void set(const glm::vec3& _p0, const glm::vec3& _p1) {
        points[0] = _p0;
        points[1] = _p1;
        direction = points[1] - points[0];
    }
    
    const glm::vec3& operator[](size_t _index) const { return points[_index]; }
    const glm::vec3& getPoint(size_t _index) const { return points[_index]; }
    glm::vec3        getCentroid() const { return (points[0] + points[1]) * 0.5f; }
    float            getMagintude() const { return glm::length(direction); }
    const glm::vec3& getDireciton() const { return direction; }
    glm::vec3        getAt(float _t) const { return points[0] + direction * _t; }
    

private:
    glm::vec3   points[2];
    glm::vec3   direction;
};

typedef std::shared_ptr<Line> LinePtr;
typedef std::shared_ptr<Line const> LineConstPtr;

}