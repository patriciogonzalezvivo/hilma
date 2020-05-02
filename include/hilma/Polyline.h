#pragma once

#include <vector>

#include "glm/glm.hpp"

namespace hilma {

class Polyline {
public:

    Polyline();
    Polyline(const Polyline& _poly);
    Polyline(const glm::vec3* _points, int _n);
    Polyline(const float* _data, int _m, int _n);
    virtual ~Polyline();


    void                    append(const Polyline& _poly);

    void                    addPoint(const glm::vec3& _point);
    void                    addPoint(const float* _data, int _n);

	void                    addPoints(const glm::vec3* _points, int _n);
    void                    addPoints(const float* _data, int _m, int _n);

    glm::vec3 &             operator [](const int &_index);
    const glm::vec3 &       operator [](const int &_index) const;

    const std::vector<glm::vec3> & getPoints() const;
    glm::vec3               getPointAt(const float &_dist) const;

    bool                    isInside(float _x, float _y);

    size_t                  size() const { return m_points.size(); }
    bool                    empty() const { return size() == 0; }

    std::vector<Polyline>   splitAt(float _dist);

    void                    clear();
    
protected:
    std::vector<glm::vec3>  m_points;
};

}