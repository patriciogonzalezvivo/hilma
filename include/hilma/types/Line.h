#pragma once 

#include <memory>
#include <vector>
#include "glm/glm.hpp"

namespace hilma {

class Line{
public:
    Line();
    Line(const glm::vec3& _p0, const glm::vec3& _p1);
    
    void set(const glm::vec3& _p0, const glm::vec3& _p1);
    
    float               getMagintude() const { return glm::length(direction); }
    glm::vec3           getCentroid() const { return (points[0] + points[1]) * 0.5f; }
    const glm::vec3&    getDireciton() const { return direction; }

    glm::vec3           getAt(float _t) const { return points[0] + direction * _t; }
    const glm::vec3&    getPoint(size_t _index) const { return points[_index]; }
    const glm::vec3&    operator[](size_t _index) const { return points[_index]; }

    bool                haveColors() const { return !colors.empty(); }
    void                setColor(const glm::vec4 &_color);
    void                setColor(float _r, float _g, float _b, float _a = 1.0f);
    void                setColor(size_t _index, const glm::vec4& _color);
    const glm::vec4&    getColor(size_t _index) const { return colors[_index]; }
    glm::vec4           getColorAt(float _t) const;
    

private:
    std::vector<glm::vec4>  colors;
    glm::vec3               points[2];
    glm::vec3               direction;
};

typedef std::shared_ptr<Line> LinePtr;
typedef std::shared_ptr<Line const> LineConstPtr;

}