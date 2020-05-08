#pragma once

#include "glm/glm.hpp"

namespace hilma {

class Triangle {
public :
    
    Triangle();
    Triangle(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2);
    virtual ~Triangle();

    void set(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2);
    void setColors(const glm::vec4 &_p0, const glm::vec4 &_p1, const glm::vec4 &_p2);
    void setNormals(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2);
    void setTexCoords(const glm::vec2 &_p0, const glm::vec2 &_p1, const glm::vec2 &_p2);
    bool containsPoint(const glm::vec3 &_p);
    
    // Getters:
    const glm::vec3& getPoint(size_t _index) const { return points[_index]; }
    const glm::vec3& operator[] (size_t _index) const { return points[_index]; }

    glm::vec3   getCentroid() const { return (points[0] + points[1] + points[2]) * 0.3333333333333f; }
    const glm::vec3& getNormal() const { return normal; }
    
    bool        haveColors() const { return colors != nullptr; }
    glm::vec4   getColor(size_t _index) { return colors[_index]; }

    bool        haveNormals() const { return normals != nullptr; }
    glm::vec3   getNormal(size_t _index) { return normals[_index]; }

    bool        haveTexCoords() const { return texcoords != nullptr; }
    glm::vec2   getTexCoord(size_t _index) { return texcoords[_index]; }
    
private:
    glm::vec3   points[3];
    glm::vec3   normal;

    glm::vec4*  colors;
    glm::vec3*  normals;
    glm::vec2*  texcoords;
};

}