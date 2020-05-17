#pragma once

#include "hilma/math.h"

namespace hilma {

class Ray {
public:
    
    Ray(): origin(0.0,0.0,0.0), direction(1.0,0.0,0.0) { };
    Ray(const glm::vec3& _org, const glm::vec3& _dir): origin(_org), direction(_dir){};
    
    void set(const glm::vec3& _org, const glm::vec3& _dir){
        origin = _org;
        direction = _dir;
        invDirection = 1.0f / _dir; 
    }
    
    const glm::vec3& getOrigin() const { return origin;}
    const glm::vec3& getDirection() const { return direction;}
    const glm::vec3& getInvertDirection() const { return invDirection; }
    
    glm::vec3 getAt(float _t) const { return origin + direction * _t; }
    
private:
    glm::vec3 origin;
    glm::vec3 direction;
    glm::vec3 invDirection;
};

}