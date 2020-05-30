#pragma once

#include <vector>
#include <memory>

#include "glm/glm.hpp"
#include "hilma/types/Material.h"

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
    void setTangents(const glm::vec4 &_p0, const glm::vec4 &_p1, const glm::vec4 &_p2);

    bool containsPoint(const glm::vec3 &_p);
    
    // Getters:
    const glm::vec3&    operator[](size_t _index) const { return vertices[_index]; }
    void                setVertex(size_t _index, const glm::vec3& _vertex);
    const glm::vec3&    getVertex(size_t _index) const { return vertices[_index]; }
    glm::vec3           getVertex(const glm::vec3& _barycenterCoord ) const;

    glm::vec3           getCentroid() const { return (vertices[0] + vertices[1] + vertices[2]) * 0.3333333333333f; }
    glm::vec3           getBarycentricOf( const glm::vec3& _p ) const;
    
    bool                haveColors() const { return !colors.empty(); }
    void                setColor(const glm::vec4 &_color);
    void                setColor(float _r, float _g, float _b, float _a = 1.0f);
    void                setColor(size_t _index, const glm::vec4& _color);
    const glm::vec4&    getColor(size_t _index) const { return colors[_index]; }
    glm::vec4           getColor(const glm::vec3& _barycenterCoord ) const;

    bool                haveNormals() const { return !normals.empty(); }
    void                setNormal(size_t _index, const glm::vec3& _normal);
    const glm::vec3&    getNormal() const { return normal; }
    const glm::vec3&    getNormal(size_t _index) const { return normals[_index]; }
    glm::vec3           getNormal(const glm::vec3& _barycenterCoord ) const;

    bool                haveTexCoords() const { return !texcoords.empty(); }
    void                setTexCoord(size_t _index, const glm::vec2& _texcoord);
    const glm::vec2&    getTexCoord(size_t _index) const { return texcoords[_index]; }
    glm::vec2           getTexCoord(const glm::vec3& _barycenterCoord ) const;

    bool                haveTangents() const { return !tangents.empty(); }
    void                setTangent(size_t _index, const glm::vec4& _tangent);
    const glm::vec4&    getTangent(size_t _index) const { return tangents[_index]; }
    glm::vec4           getTangent(const glm::vec3& _barycenterCoord ) const;
    
    MaterialConstPtr    material = nullptr;

    static bool         compare(const Triangle& a, const Triangle& b, int axis) {
        return a.getCentroid()[axis] < b.getCentroid()[axis];
    }

    static bool         compareX (const Triangle& a, const Triangle& b) { return compare(a, b, 0); }
    static bool         compareY (const Triangle& a, const Triangle& b) { return compare(a, b, 1); }
    static bool         compareZ (const Triangle& a, const Triangle& b) { return compare(a, b, 2); }
    
private:
    glm::vec3               vertices[3];
    glm::vec3               normal;
    float                   area;

    std::vector<glm::vec4>  colors;
    std::vector<glm::vec3>  normals;
    std::vector<glm::vec2>  texcoords;
    std::vector<glm::vec4>  tangents;
    
};

typedef std::shared_ptr<Triangle> TrianglePtr;
typedef std::shared_ptr<Triangle const> TriangleConstPtr;

}