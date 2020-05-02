#include "hilma/Polyline.h"

#include <algorithm>
#include "hilma/math.h"

using namespace hilma;


Polyline::Polyline() {
    m_points.clear();
}

Polyline::Polyline(const Polyline &_poly) {
    append(_poly);
}

Polyline::Polyline(const glm::vec3* _points, int _n) {
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
    
    addPoints( (const glm::vec3*)&_poly.m_points, _poly.size());
}

void Polyline::addPoint( const glm::vec3& _point ){
    m_points.push_back(_point);
}

void Polyline::addPoint( const float* _data, int _n ){
    glm::vec3 v;
    for (int i = 0; i < 3 && i < _n; i++)
        v[i] = _data[i];
    m_points.push_back(v);
}

void Polyline::addPoints(const glm::vec3* _points, int _n) {
    m_points.insert(m_points.end(), _points, _points + _n);
}

void Polyline::addPoints(const float* _data, int _m, int _n) {
    for (int i = 0 ; i < _m ; i++)
        addPoint(&_data[i*_n], _n);
}

glm::vec3 & Polyline::operator [](const int &_index){
    return m_points[_index];
}

const glm::vec3 & Polyline::operator [](const int &_index) const {
    return m_points[_index];
}

const std::vector<glm::vec3> & Polyline::getPoints() const{
	return m_points;
}

glm::vec3 Polyline::getPointAt(const float &_dist) const{
    float distance = 0.0;
    for (size_t i = 0; i < m_points.size()-1; i++){
        glm::vec3 polar = toPolar(m_points[i], m_points[i+1]);
        if (distance + polar.y <= _dist){
            return  m_points[i] + toCartesian(glm::vec3(polar.x, _dist-distance, polar.z));
        }
        distance += polar.y;
	}
    return glm::vec3(0.0f);
}

std::vector<Polyline> Polyline::splitAt(float _dist){
    std::vector<Polyline> RTA;
    if (size()>0) {
        Polyline buffer;

        buffer.addPoint(m_points[0]);
        float distance = 0.0;
        for (size_t i = 0; i < m_points.size() - 1; i++) {
            glm::vec3 polar = toPolar(m_points[i], m_points[i+1]);
            if(distance+polar.r <= _dist){
                glm::vec3 cutPoint =  toCartesian(glm::vec3(polar.x, _dist-distance, polar.z));
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

// http://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
//
bool Polyline::isInside(float _x, float _y){
	int counter = 0;
	double xinters;
    glm::vec3 p1,p2;

	int N = size();
	p1 = m_points[0];
	for (int i = 1; i <= N; i++) {
		p2 = m_points[i % N];
		if (_y > std::min(p1.y, p2.y)) {
            if (_y <= std::max(p1.y, p2.y)) {
                if (_x <= std::max(p1.x, p2.x)) {
                    if (p1.y != p2.y) {
                        xinters = (_y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;
                        if (p1.x == p2.x || _x <= xinters)
                            counter++;
                    }
                }
            }
		}
		p1 = p2;
	}

	if (counter % 2 == 0) return false;
	else return true;
}