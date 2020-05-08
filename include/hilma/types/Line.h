#pragma once 

#include "glm/glm.hpp"

namespace hilma {

class Line{
public:
    Line() : vec(0.0) { };
    Line(glm::vec3 _p0, glm::vec3 _p1) {
        set(_p0,_p1);
    }
    
    void set(glm::vec3 _p0, glm::vec3 _p1) {
        points[0]=_p0;
        points[1]=_p1;
        vec = points[1] - points[0];
    }
    
    const glm::vec3& operator[] (size_t _index) const { return points[_index]; }
    float       getMagintude() const { return glm::length(vec); }
    glm::vec3   getVector() const { return vec; }

private:
    glm::vec3   points[2];
    glm::vec3   vec;
};

}