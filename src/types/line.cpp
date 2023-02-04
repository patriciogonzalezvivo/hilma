#include "hilma/types/line.h"

namespace hilma {
Line::Line() : direction(0.0) { 

}

Line::Line(const glm::vec3& _p0, const glm::vec3& _p1) {
    set(_p0,_p1);
}
    
void Line::set(const glm::vec3& _p0, const glm::vec3& _p1) {
    points[0] = _p0;
    points[1] = _p1;
    direction = points[1] - points[0];
}

void Line::setColor(const glm::vec4 &_color) {
    colors.resize(2);
    colors[0] = _color;
    colors[1] = _color;
}

void Line::setColor(float _r, float _g, float _b, float _a) {
    setColor(glm::vec4(_r, _g, _b, _a));
}

void Line::setColor(size_t _index, const glm::vec4& _color) {
    if (colors.empty()) {
        colors.resize(2);
        std::fill(colors.begin(),colors.begin()+2, _color);
    }
    else
        colors[_index] = _color;
}

glm::vec4 Line::getColorAt(float _t) const {
    if (haveColors())
        return colors[0] * (1.0f-_t) + colors[1] * _t;
    else
        return glm::vec4(0.0); 
}

}