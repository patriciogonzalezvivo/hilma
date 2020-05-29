#pragma once

#include <string>
#include <vector>
#include <utility>
#include <map>

#include "hilma/types/Ray.h"
#include "hilma/types/Line.h"
#include "hilma/types/Triangle.h"
#include "hilma/types/Material.h"
#include "hilma/types/BoundingBox.h"

namespace hilma {

enum FaceType {
    POINTS          = 0,
    TRIANGLE_STRIP  = 1,
    TRIANGLE_FAN    = 2,
    TRIANGLES       = 3,
    QUAD            = 4
};

enum EdgeType {
    LINE_STRIP  = 0,
    LINES       = 1
};

#if defined(PLATFORM_RPI)
#define INDEX_TYPE uint16_t
#else
#define INDEX_TYPE uint32_t
#endif

typedef std::pair<size_t, MaterialPtr> IndexMaterial;
typedef std::vector<IndexMaterial> MaterialsByIndices;
typedef std::map<std::string, MaterialPtr> MaterialsByName;

class Mesh {
public:

    Mesh();
    Mesh(const Mesh& _mother);
    Mesh(const std::string& _name);
    virtual ~Mesh();

    void        clear();
    void        append(const Mesh& _mesh);

    void        setFaceType(FaceType _mode = TRIANGLES, bool _compute = false);
    FaceType    getFaceType() const { return faceMode; };

    void        setEdgeType(EdgeType _mode = LINES, bool _compute = false);
    EdgeType    getEdgeType() const { return edgeMode;};
    
    void            setName(const std::string& _name) { name = _name; }
    std::string     getName() const { return name; }

    // Vertices
    void        addVertex(const glm::vec3& _point);
    void        addVertex(const float* _array1D, int _n);
    void        addVertex(float _x, float _y, float _z = 0.0f);

    void        addVertices(const float* _array2D, int _m, int _n);

    bool        haveVertices() const { return !vertices.empty(); };
    void        clearVertices() { vertices.clear(); }

    const size_t        getVerticesTotal() const { return vertices.size(); }
    const glm::vec3&    getVertex(size_t _index) const { return vertices[_index]; }
    const std::vector<glm::vec3>& getVertices() const { return vertices; }

    // Colorss
    void        setColor(const glm::vec4& _color);
    void        setColor(const float* _array1D, int _n);
    void        setColor(float _r, float _g, float _b, float _a = 1.0f);
    void        clearColors() { colors.clear(); }

    void        addColor(const glm::vec4& _color);
    void        addColor(const float* _array1D, int _n);
    void        addColor(float _r, float _g, float _b, float _a = 1.0f);

    void        addColors(const float* _array2D, int _m, int _n);

    const bool  haveColors() const { return !colors.empty(); }
    size_t      getColorsTotal() const { return colors.size(); }
    const std::vector<glm::vec4>& getColors() const { return colors; }

    // Normals
    void        addNormal(const glm::vec3& _normal);
    void        addNormal(const float* _array1D, int _n);
    void        addNormal(float _nX, float _nY, float _nZ);

    void        addNormals(const float* _array2D, int _m, int _n);

    const bool  haveNormals() const { return !normals.empty(); }
    const glm::vec3&   getNormal(size_t _index) const { return normals[_index]; }
    size_t      getNormalsTotal() const { return normals.size(); }
    const std::vector<glm::vec3>& getNormals() const { return normals; }
    void        clearNormals() { normals.clear(); }
    bool        computeNormals();
    void        smoothNormals(float _angle);
    void        invertNormals();
    void        flatNormals();

    // TexCoord
    void        addTexCoord(const glm::vec2& _uv);
    void        addTexCoord(float _tX, float _tY);
    void        addTexCoord(const float* _array1D, int _n);
    void        addTexCoords(const float* _array2D, int _m, int _n);

    const bool  haveTexCoords() const { return !texcoords.empty(); }
    size_t      getTexCoordsTotal() const { return texcoords.size(); }
    const glm::vec2&   getTexCoord(size_t _index) const { return texcoords[_index]; }
    const std::vector<glm::vec2>& getTexCoords() const { return texcoords; }
    void        clearTexCoords() { texcoords.clear(); }

    // Tangents
    const bool  haveTangents() const { return !tangents.empty(); }
    const glm::vec4&   getTangent(size_t _index) const { return tangents[_index]; }
    const std::vector<glm::vec4>& getTangents() const { return tangents; }
    bool        computeTangents();
    void        clearTangets() { tangents.clear(); }

    void        addIndices(const INDEX_TYPE* _array2D, int _m, int _n);
    void        mergeDuplicateVertices();

    // FACES
    void        addTriangle(const Triangle& _tri);
    void        addTriangleIndices(INDEX_TYPE _i1, INDEX_TYPE _i2, INDEX_TYPE _i3);
    void        addQuadIndices(INDEX_TYPE _i1, INDEX_TYPE _i2, INDEX_TYPE _i3, INDEX_TYPE _i4);

    void        addFaceIndex(INDEX_TYPE _i);
    void        addFaceIndices(const INDEX_TYPE* _array1D, int _n);
    size_t      getFaceIndex(size_t _index);  

    void        addTriangles(const Triangle* _array1D, int _n);

    std::vector<Triangle>   getTriangles() const;
    std::vector<glm::ivec3> getTrianglesIndices() const;
    
    const bool  haveFaceIndices() const { return !faceIndices.empty(); }
    size_t      getFaceIndicesTotal() const { return faceIndices.size(); }
    void        clearFaceIndices() { faceIndices.clear(); }
    void        invertWindingOrder();

    // EDGES
    void        addEdgeIndex(INDEX_TYPE _i);
    void        addEdgeIndices(const INDEX_TYPE* _array1D, int _n);

    void        addEdge(const Line& _line);
    void        addEdges(const Line* _array1D, int _n);
    void        addEdgeIndices(INDEX_TYPE _i1, INDEX_TYPE _i2);

    const bool  haveEdgeIndices() const { return !edgeIndices.empty(); }
    size_t      getEdgeIndicesTotal() const { return edgeIndices.size(); }
    void        clearEdgeIndices() { edgeIndices.clear(); }

    std::vector<Line>       getLinesEdges() const;
    std::vector<glm::ivec2> getLinesIndices() const;

    void        setMaterial(const Material& _material);
    void        addMaterial(const Material& _material, int _index = -1);
    MaterialPtr getMaterial(const std::string& _name) const;
    bool        haveMaterials() const { return !materialsByIndices.empty(); }
    
    void        printMaterials() const;
    std::vector<std::string> getMaterialsNames() const;
    MaterialConstPtr getMaterialForFaceIndex(size_t _index) const;

    Mesh                getMeshForIndices(size_t _start, size_t _end) const;
    std::vector<Mesh>   getMeshesByMaterials() const;

private:
    MaterialsByName         materialsByName;
    MaterialsByIndices      materialsByIndices;
    
    std::vector<glm::vec4>  colors;
    std::vector<glm::vec4>  tangents;
    std::vector<glm::vec3>  vertices;
    std::vector<glm::vec3>  normals;
    std::vector<glm::vec2>  texcoords;

    std::vector<INDEX_TYPE> faceIndices;
    std::vector<INDEX_TYPE> edgeIndices;

    std::string             name;
    FaceType                faceMode;
    EdgeType                edgeMode;

    friend bool loadPly( const std::string&, Mesh& );
    friend bool savePly( const std::string&, Mesh&, bool, bool);
    friend bool loadStl( const std::string&, Mesh& );
    friend bool loadObj( const std::string&, Mesh& );
    friend bool saveObj( const std::string&, const Mesh& );

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