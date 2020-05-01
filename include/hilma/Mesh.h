#pragma once

#include <vector>
#include <string>

#include "hilma/Material.h"

namespace hilma {

enum PrimitiveType {
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
    void        add(const Mesh& _mesh);

    void        setPrimitive(PrimitiveType _primitive = TRIANGLES) { primitive = _primitive; };
    bool        computePrimitives(PrimitiveType _primitive);
    PrimitiveType   getPrimitive() const { return primitive; };

    // Vertices
    void        addVertex(const glm::vec3& _point);
    void        addVertex(const float* _data, int _n);
    void        addVertex(float _x, float _y, float _z = 0.0f);

    void        addVertices(const float* _data, int _n);
    void        addVertices(const glm::vec3* _data, int _n);
    void        addVertices(const float* _data, int _m, int _n);
    void        addVertices(const std::vector<glm::vec3>& _data);

    bool        hasVertices() const { return !vertices.empty(); };
    int         getVerticesTotal() const { return vertices.size(); }

    // Colorss
    void        setColor(const glm::vec4& _color);
    void        setColor(const float* _data, int _n);
    void        setColor(float _r, float _g, float _b, float _a = 1.0f);

    void        addColor(const glm::vec4& _color);
    void        addColor(const float* _data, int _n);
    void        addColor(float _r, float _g, float _b, float _a = 1.0f);

    void        addColors(const glm::vec4* _data, int _n);
    void        addColors(const float* _data, int _m, int _n);
    void        addColors(const std::vector<glm::vec4>& _colors);

    const bool  hasColors() const { return !colors.empty(); }
    int         getColorsTotal() const { return colors.size(); }

    // Normals
    void        addNormal(const glm::vec3& _normal);
    void        addNormal(const float* _data, int _n);
    void        addNormal(float _nX, float _nY, float _nZ);

    void        addNormals(const glm::vec3* _data, int _n);
    void        addNormals(const float* _data, int _m, int _n);
    void        addNormals(const std::vector<glm::vec3>& _normals );

    const bool  hasNormals() const { return !normals.empty(); }
    int         getNormalsTotal() const { return normals.size(); }
    
    bool        computeNormals();

    // TexCoord
    void        addTexCoord(const glm::vec2& _uv);
    void        addTexCoord(float _tX, float _tY);
    void        addTexCoord(const float* _data, int _n);

    void        addTexCoords(const glm::vec2* _data, int _n);
    void        addTexCoords(const float* _data, int _m, int _n);
    void        addTexCoords(const std::vector<glm::vec2>& _uvs);

    const bool  hasTexcoords() const { return !texcoords.empty(); }
    int         getTexCoordsTotal() const { return texcoords.size(); }

    // Tangents
    void        addTangent(const glm::vec4& _tangent);
    void        addTangent(const float* _data, int _n);
    void        addTangent(float _x, float _y, float _z, float _w);

    const bool  hasTangents() const { return !tangents.empty(); }
    bool        computeTangents();

    // Indices
    void        addIndex(int _i);

    void        addIndices(const int* _data, int _n);
    void        addIndices(const int* _data, int _m, int _n);
    void        addIndices(const std::vector<int>& _data);

    const bool  hasIndices() const { return !indices.empty(); }
    int         getIndicesTotal() const { return indices.size(); }

    void        addLine( int index1, int index2 );
    void        addTriangle(int index1, int index2, int index3);
    std::vector<glm::ivec3>  getTriangles() const;


    const bool  hasEdgeIndices() const { return !edge_indices.empty(); }
    const bool  hasEdgeColors() const { return !edge_colors.empty(); }

private:
    std::vector<Material>   materials;
    std::vector<glm::vec4>  edge_colors;
    std::vector<glm::vec4>  colors;
    std::vector<glm::vec4>  tangents;
    std::vector<glm::vec3>  vertices;
    std::vector<glm::vec3>  normals;
    std::vector<glm::vec2>  texcoords;
    std::vector<int>        indices;
    std::vector<int>        indices_normals;
    std::vector<int>        indices_texcoords;
    std::vector<int>        edge_indices;

    PrimitiveType           primitive;

    friend class PlyOps;
    friend class ObjOps;
};

}