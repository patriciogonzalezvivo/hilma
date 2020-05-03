#include "hilma/types/Polyline.h"

#include <algorithm>
#include "hilma/math.h"

using namespace hilma;


Polyline::Polyline() {
    m_points.clear();
}

Polyline::Polyline(const Polyline &_poly) {
    append(_poly);
}

Polyline::Polyline(const glm::vec2* _points, int _n) {
    addPoints(_points, _n);
}

Polyline::Polyline(const float* _data, int _m, int _n) {
    addPoints(_data, _m, _n);
}

Polyline::~Polyline(){
   clear();
}

void Polyline::clear(){
    m_points.clear();
}

void Polyline::append(const Polyline& _poly) {
    if (_poly.empty()) return;
    
    addPoints( (const glm::vec2*)&_poly.m_points, _poly.size());
}

void Polyline::addPoint( const glm::vec2& _point ){
    m_points.push_back(_point);
}

void Polyline::addPoint( const float* _data, int _n ){
    glm::vec2 v;
    for (int i = 0; i < 3 && i < _n; i++)
        v[i] = _data[i];
    m_points.push_back(v);
}

void Polyline::addPoints(const glm::vec2* _points, int _n) {
    m_points.insert(m_points.end(), _points, _points + _n);
}

void Polyline::addPoints(const float* _data, int _m, int _n) {
    for (int i = 0 ; i < _m ; i++)
        addPoint(&_data[i*_n], _n);
}

glm::vec2 & Polyline::operator [](const int &_index){
    return m_points[_index];
}

const glm::vec2 & Polyline::operator [](const int &_index) const {
    return m_points[_index];
}

const std::vector<glm::vec2> & Polyline::getPoints() const{
	return m_points;
}

glm::vec2 Polyline::getPointAt(const float &_dist) const{
    float distance = 0.0;
    for (size_t i = 0; i < m_points.size()-1; i++){
        glm::vec2 polar = toPolar(m_points[i], m_points[i+1]);
        if (distance + polar.y <= _dist)
            return  m_points[i] + toCartesian(glm::vec2(polar.x, _dist-distance));
        
        distance += polar.y;
	}
    return glm::vec2(0.0f);
}

std::vector<Polyline> Polyline::splitAt(float _dist){
    std::vector<Polyline> RTA;
    if ( size() > 0 ) {
        Polyline buffer;

        buffer.addPoint(m_points[0]);
        float distance = 0.0;
        for (size_t i = 0; i < m_points.size() - 1; i++) {
            glm::vec2 polar = toPolar(m_points[i], m_points[i+1]);
            if ( distance + polar.r <= _dist ) {
                glm::vec2 cutPoint =  toCartesian(glm::vec2(polar.x, _dist-distance));
                buffer.addPoint(m_points[i] + cutPoint);
                RTA.push_back(buffer);
                buffer.clear();
                buffer.addPoint(m_points[i] + cutPoint);
                break;
            }
            buffer.addPoint(m_points[i+1]);
            distance += polar.r;
        }
        RTA.push_back(buffer);
    }
    return RTA;
}
