#pragma once

#include <vector>

#include "hilma/types/Material.h"
#include "hilma/types/BoundingBox.h"

namespace hilma {

enum PrimitiveMode {
    UNKNOWN     = 0,
    POINTS      = 1,
    LINES       = 2,
    TRIANGLES   = 3,
    LINE_STRIP,
    TRIANGLE_STRIP,
    TRIANGLE_FAN
};

#if defined(PLATFORM_RPI)
#define INDEX_TYPE uint16_t
#else
#define INDEX_TYPE uint32_t
#endif


class Mesh {
public:

    Mesh();
    Mesh(const Mesh& _mother);
    virtual ~Mesh();

    void        clear();
    void        append(const Mesh& _mesh);

    void        setMode(PrimitiveMode _mode = TRIANGLES, bool _compute = false);
    PrimitiveMode    getMode() const { return m_mode; };

    // Vertices
    void        addVertex(const glm::vec3& _point);
    void        addVertex(const float* _array1D, int _n);
    void        addVertex(float _x, float _y, float _z = 0.0f);

    void        addVertices(const float* _array2D, int _m, int _n);

    void        clearVertices() { m_vertices.clear(); }
    bool        hasVertices() const { return !m_vertices.empty(); };
    size_t      getVerticesTotal() const { return m_vertices.size(); }
    std::vector<glm::vec3> getVertices() const { return m_vertices; }

    // Colorss
    void        setColor(const glm::vec4& _color);
    void        setColor(const float* _array1D, int _n);
    void        setColor(float _r, float _g, float _b, float _a = 1.0f);
    void        clearColors() { m_colors.clear(); }

    void        addColor(const glm::vec4& _color);
    void        addColor(const float* _array1D, int _n);
    void        addColor(float _r, float _g, float _b, float _a = 1.0f);

    void        addColors(const float* _array2D, int _m, int _n);

    const bool  hasColors() const { return !m_colors.empty(); }
    size_t      getColorsTotal() const { return m_colors.size(); }
    std::vector<glm::vec4> getColors() const { return m_colors; }

    // Normals
    void        addNormal(const glm::vec3& _normal);
    void        addNormal(const float* _array1D, int _n);
    void        addNormal(float _nX, float _nY, float _nZ);

    void        addNormals(const float* _array2D, int _m, int _n);

    const bool  hasNormals() const { return !m_normals.empty(); }
    size_t      getNormalsTotal() const { return m_normals.size(); }
    std::vector<glm::vec3> getNormals() const { return m_normals; }
    void        clearNormals() { m_normals.clear(); }
    bool        computeNormals();
    void        invertNormals();
    void        flatNormals();

    // TexCoord
    void        addTexCoord(const glm::vec2& _uv);
    void        addTexCoord(float _tX, float _tY);
    void        addTexCoord(const float* _array1D, int _n);

    void        addTexCoords(const float* _array2D, int _m, int _n);

    const bool  hasTexCoords() const { return !m_texcoords.empty(); }
    size_t      getTexCoordsTotal() const { return m_texcoords.size(); }
    std::vector<glm::vec2> getTexCoords() const { return m_texcoords; }
    void        clearTexCoords() { m_texcoords.clear(); }

    // Tangents
    void        addTangent(const glm::vec4& _tangent);
    void        addTangent(const float* _array1D, int _n);
    void        addTangent(float _x, float _y, float _z, float _w);

    const bool  hasTangents() const { return !m_tangents.empty(); }
    std::vector<glm::vec4> getTangents() const { return m_tangents; }
    bool        computeTangents();
    void        clearTangets() { m_tangents.clear(); }

    // Indices
    void        addIndex(INDEX_TYPE _i);
    void        addIndices(const INDEX_TYPE* _array1D, int _n);

    const bool  hasIndices() const { return !m_indices.empty(); }
    size_t      getIndicesTotal() const { return m_indices.size(); }

    void        addFaces(const INDEX_TYPE* _array2D, int _m, int _n);
    void        addLine(INDEX_TYPE _i1, INDEX_TYPE _i2);
    void        addTriangle(INDEX_TYPE _i1, INDEX_TYPE _i2, INDEX_TYPE _i3);
    void        clearIndices() { m_indices.clear(); }
    void        invertWindingOrder();
    void        mergeDuplicateVertices();

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
    std::vector<INDEX_TYPE> m_indices;
    // std::vector<INDEX_TYPE> m_indices_normals;
    // std::vector<INDEX_TYPE> m_indices_texcoords;
    // std::vector<INDEX_TYPE> m_edge_indices;
    // std::vector<glm::vec4>  m_edge_colors;

    PrimitiveMode                m_mode;

    friend bool loadPly( const std::string&, Mesh& );
    friend bool savePly( const std::string&, Mesh&, bool);
    friend bool saveObj( const std::string&, Mesh& );

    friend void scale(Mesh&, float );
    friend void scaleX(Mesh&, float );
    friend void scaleY(Mesh&, float );
    friend void scaleZ(Mesh&, float );
    friend void scale(Mesh&, const glm::vec3& );
    friend void scale(Mesh&, float , float , float );

    friend void translateX(Mesh&, float );
    friend void translateY(Mesh&, float );
    friend void translateZ(Mesh&, float );
    friend void translate(Mesh&, const glm::vec3& );
    friend void translate(Mesh&, float , float , float );

    friend void rotateX(Mesh&, float );
    friend void rotateY(Mesh&, float );
    friend void rotateZ(Mesh&, float );
    friend void rotate(Mesh&, float, const glm::vec3& );
    friend void rotate(Mesh&, float , float, float, float );

    friend void center(Mesh&);

    friend BoundingBox getBoundingBox(const Mesh&);
};

}