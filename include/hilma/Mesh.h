#pragma once

#include <vector>
#include <string>

#include "hilma/Material.h"

namespace hilma {

enum ModeType {
    UNKNOWN     = 0,
    POINTS      = 1,
    LINES       = 2,
    TRIANGLES   = 3 //,
    // QUADS       = 4,
    // LINE_STRIP  = 5,
    // TRIANGLES_STRIP = 6,
    // TRIANGLES_FAN   = 7
};

class Mesh {
public:

    Mesh();
    Mesh(const Mesh& _mother);
    virtual ~Mesh();

    void        clear();
    void        append(const Mesh& _mesh);

    void        setMode(ModeType _mode = TRIANGLES, bool _compute = false);
    ModeType    getMode() const { return m_mode; };

    // Vertices
    void        addVertex(const glm::vec3& _point);
    void        addVertex(const float* _data, int _n);
    void        addVertex(float _x, float _y, float _z = 0.0f);

    void        addVertices(const float* _data, int _n);
    void        addVertices(const float* _data, int _m, int _n);

    bool        hasVertices() const { return !m_vertices.empty(); };
    int         getVerticesTotal() const { return m_vertices.size(); }

    // Colorss
    void        setColor(const glm::vec4& _color);
    void        setColor(const float* _data, int _n);
    void        setColor(float _r, float _g, float _b, float _a = 1.0f);

    void        addColor(const glm::vec4& _color);
    void        addColor(const float* _data, int _n);
    void        addColor(float _r, float _g, float _b, float _a = 1.0f);

    void        addColors(const float* _data, int _m, int _n);

    const bool  hasColors() const { return !m_colors.empty(); }
    int         getColorsTotal() const { return m_colors.size(); }

    // Normals
    void        addNormal(const glm::vec3& _normal);
    void        addNormal(const float* _data, int _n);
    void        addNormal(float _nX, float _nY, float _nZ);

    void        addNormals(const float* _data, int _m, int _n);

    const bool  hasNormals() const { return !m_normals.empty(); }
    int         getNormalsTotal() const { return m_normals.size(); }
    bool        computeNormals();

    // TexCoord
    void        addTexCoord(const glm::vec2& _uv);
    void        addTexCoord(float _tX, float _tY);
    void        addTexCoord(const float* _data, int _n);

    void        addTexCoords(const float* _data, int _m, int _n);

    const bool  hasTexcoords() const { return !m_texcoords.empty(); }
    int         getTexCoordsTotal() const { return m_texcoords.size(); }

    // Tangents
    void        addTangent(const glm::vec4& _tangent);
    void        addTangent(const float* _data, int _n);
    void        addTangent(float _x, float _y, float _z, float _w);

    const bool  hasTangents() const { return !m_tangents.empty(); }
    bool        computeTangents();

    // Indices
    void        addIndex(int _i);

    void        addIndices(const int* _data, int _n);
    void        addIndices(const int* _data, int _m, int _n);

    const bool  hasIndices() const { return !m_indices.empty(); }
    int         getIndicesTotal() const { return m_indices.size(); }

    void        addLine( int index1, int index2 );
    void        addTriangle(int index1, int index2, int index3);
    std::vector<glm::ivec3>  getTriangles() const;


    // const bool  hasEdgeIndices() const { return !m_edge_indices.empty(); }
    // const bool  hasEdgeColors() const { return !m_edge_colors.empty(); }

private:
    std::vector<Material>   m_materials;
    std::vector<glm::vec4>  m_colors;
    std::vector<glm::vec4>  m_tangents;
    std::vector<glm::vec3>  m_vertices;
    std::vector<glm::vec3>  m_normals;
    std::vector<glm::vec2>  m_texcoords;
    std::vector<int>        m_indices;
    // std::vector<int>        m_indices_normals;
    // std::vector<int>        m_indices_texcoords;
    // std::vector<int>        m_edge_indices;
    // std::vector<glm::vec4>  m_edge_colors;

    ModeType                m_mode;

    friend class PlyOps;
    friend class ObjOps;
};

}