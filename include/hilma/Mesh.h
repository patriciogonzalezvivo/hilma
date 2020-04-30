#pragma once

#include <vector>
#include <string>

#include "hilma/Material.h"

#if defined(PLATFORM_RPI) || defined(PLATFORM_RPI4) 
#define INDEX_TYPE uint16_t
#else
#define INDEX_TYPE uint32_t
#endif

namespace hilma {

enum Primitive {
    POINTS,
    LINES,
    LINE_STRIP,
    TRIANGLES,
    TRIANGLES_STRIP,
    TRIANGLES_FAN
};

class Mesh {
public:

    Mesh();
    Mesh(const Mesh& _mother);
    virtual ~Mesh();

    void        add(const Mesh& _mesh);

    void        setPrimitive(Primitive _primitive = TRIANGLES) { primitive = _primitive; };
    Primitive   getPrimitive() const { return primitive; };


    void        addVertex(const glm::vec3& _point);
    void        addVertex(const float* _data, int _n);
    void        addVertex(float _x, float _y, float _z = 0.0f);

    void        addVertices(const std::vector<glm::vec3>& _verts);
    void        addVertices(const glm::vec3* _verts, int _amt);

    void        setColor(const glm::vec4& _color);
    void        setColor(const float* _data, int _n);
    void        setColor(float _r, float _g, float _b, float _a = 1.0f);
    void        addColor(const glm::vec4& _color);
    void        addColor(const float* _data, int _n);
    void        addColor(float _r, float _g, float _b, float _a = 1.0f);
    void        addColors(const std::vector<glm::vec4>& _colors);

    void        addNormal(const glm::vec3& _normal);
    void        addNormal(const float* _data, int _n);
    void        addNormal(float _nX, float _nY, float _nZ);
    void        addNormals(const std::vector<glm::vec3>& _normals );

    void        addTangent(const glm::vec4& _tangent);
    void        addTangent(const float* _data, int _n);
    void        addTangent(float _x, float _y, float _z, float _w);

    void        addTexcoord(const glm::vec2& _uv);
    void        addTexcoord(float _tX, float _tY);
    void        addTexcoord(const float* _data, int _n);
    void        addTexcoords(const std::vector<glm::vec2>& _uvs);

    void        addIndex(INDEX_TYPE _i);
    void        addIndices(const INDEX_TYPE* _inds, int _amt);
    void        addIndices(const std::vector<INDEX_TYPE>& _inds);

    void        addTriangle(INDEX_TYPE index1, INDEX_TYPE index2, INDEX_TYPE index3);
    std::vector<glm::ivec3>  getTriangles() const ;

    const bool  hasColors() const { return colors.size() > 0; }
    const bool  hasNormals() const { return normals.size() > 0; }
    const bool  hasTexcoords() const { return texcoords.size() > 0; }
    const bool  hasTangents() const { return tangents.size() > 0; }
    const bool  hasIndices() const { return indices.size() > 0; }
    const bool  hasEdgeIndices() const { return edge_indices.size() > 0; }
    const bool  hasEdgeColors() const { return edge_colors.size() > 0; }

    bool        computeNormals();
    bool        computeTangents();
    void        clear();

    std::vector<Material>   materials;
    std::vector<glm::vec4>  edge_colors;
    std::vector<glm::vec4>  colors;
    std::vector<glm::vec4>  tangents;
    std::vector<glm::vec3>  vertices;
    std::vector<glm::vec3>  normals;
    std::vector<glm::vec2>  texcoords;
    std::vector<INDEX_TYPE> indices;
    std::vector<INDEX_TYPE> indices_normals;
    std::vector<INDEX_TYPE> indices_texcoords;
    std::vector<INDEX_TYPE> edge_indices;

private:
    Primitive               primitive;
};

}