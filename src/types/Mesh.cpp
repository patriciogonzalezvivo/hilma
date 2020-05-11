#include <iostream>
#include <map>

#include "hilma/types/Mesh.h"

using namespace hilma;

Mesh::Mesh() : name("undefined"), faceMode(TRIANGLES), edgeMode(LINES) {
}

Mesh::Mesh(const std::string& _name) : name(_name), faceMode(TRIANGLES), edgeMode(LINES)  {
}

Mesh::Mesh(const Mesh& _mother): name(_mother.name), faceMode(_mother.faceMode), edgeMode(_mother.edgeMode) {
    append(_mother);
}


Mesh::~Mesh() {

}

void Mesh::clear() {
    if (!vertices.empty()) vertices.clear();
    
    if (!colors.empty()) colors.clear();
    if (!normals.empty()) normals.clear();
    if (!texcoords.empty()) texcoords.clear();
    if (!tangents.empty()) tangents.clear();
    if (!faceIndices.empty()) faceIndices.clear();
    if (!edgeIndices.empty()) edgeIndices.clear();
    // if (!indices_normals.empty()) indices_normals.clear();
    // if (!indices_texcoords.empty()) indices_texcoords.clear();
    // if (!edge_colors.empty()) edge_colors.clear();
    // if (!edge_faceIndices.empty()) edge_faceIndices.clear();
}

void Mesh::append(const Mesh& _mesh) {
    int indexOffset = (int)vertices.size();

    if (_mesh.haveVertices())
        vertices.insert(vertices.end(), _mesh.vertices.begin(), _mesh.vertices.end());
        
    if (_mesh.haveTexCoords())
        texcoords.insert(texcoords.end(), _mesh.texcoords.begin(), _mesh.texcoords.end());
        
    if (_mesh.haveColors())
        colors.insert(colors.end(),_mesh.colors.begin(),_mesh.colors.end());
        
    if (_mesh.haveNormals())
        normals.insert(normals.end(),_mesh.normals.begin(),_mesh.normals.end());

    if (_mesh.getFaceType() != faceMode) {
        std::cout << "INCOMPATIBLE FACEMODES" << std::endl;
        return;
    }

    if (_mesh.getEdgeType() != edgeMode) {
        std::cout << "INCOMPATIBLE FACEMODES" << std::endl;
        return;
    }

    if (_mesh.haveFaceIndices())
        for (size_t i = 0; i < _mesh.faceIndices.size(); i++)
            addFaceIndex(indexOffset + _mesh.faceIndices[i]);

    if (_mesh.haveEdgeIndices())
        for (size_t i = 0; i < _mesh.edgeIndices.size(); i++)
            addEdgeIndex(indexOffset + _mesh.edgeIndices[i]);
}

// Vertices
//
void Mesh::addVertex(const glm::vec3& _point) {
   vertices.push_back(_point);
}

void Mesh::addVertex(float _x, float _y, float _z) {
    addVertex( glm::vec3(_x, _y, _z) );
}

void Mesh::addVertex(const float* _data, int _n) {
    if (_n == 3)
        addVertex(_data[0], _data[1], _data[2]);
    else if (_n == 2)
        addVertex(_data[0], _data[1], 0.0);
}

void Mesh::addVertices(const float* _data, int _m, int _n) {
    for (int i = 0; i < _m; i++)
        addVertex(&_data[i*_n], _n);
}


// Color
//
void Mesh::setColor(const glm::vec4& _color) {
    colors.clear();
    for (size_t i = 0; i < vertices.size(); i++)
        colors.push_back(_color);
}

void Mesh::setColor(const float* _data, int _n) {
    if (_n == 4)
        setColor(_data[0], _data[1], _data[2], _data[3]);
    else if (_n == 3)
        setColor(_data[0], _data[1], _data[2], 1.0);
}

void Mesh::setColor(float _r, float _g, float _b, float _a) {
    setColor(glm::vec4(_r, _g, _b, _a));
}

void Mesh::addColor(const glm::vec4& _color) {
    colors.push_back(_color);
}

void Mesh::addColor(const float* _data, int _n) {
    if (_n == 4)
        addColor(_data[0], _data[1], _data[2], _data[3]);
    else if (_n == 3)
        addColor(_data[0], _data[1], _data[2], 1.0);
}

void Mesh::addColor(float _r, float _g, float _b, float _a) {
    addColor(glm::vec4(_r, _g, _b, _a));
}

void Mesh::addColors(const float* _data, int _m, int _n) {
    for (int i = 0; i < _m; i++)
        addColor(&_data[i*_n], _n);
}

// Normals
//
void Mesh::addNormal(const glm::vec3& _normal) {
    normals.push_back(_normal);
}

void Mesh::addNormal(const float* _data, int _n) {
    if (_n != 3) return;

    addNormal(_data[0], _data[1], _data[2]);
}

void Mesh::addNormal(float _nX, float _nY, float _nZ) {
    addNormal( glm::vec3(_nX, _nY, _nZ) );
}

void Mesh::addNormals(const float* _data, int _m, int _n) {
    for (int i = 0; i < _m; i++)
        addNormal(&_data[i*_n], _n);
}


// Tangents
//
void  Mesh::addTangent(const glm::vec4& _tangent) {
    tangents.push_back(_tangent);
}

void Mesh::addTangent(const float* _data, int _n) {
    if (_n == 4)
        addTangent(_data[0], _data[1], _data[2], _data[3]);
}

void Mesh::addTangent(float _x, float _y, float _z, float _w) {
    tangents.push_back( glm::vec4(_x, _y, _z, _w) );
}


// TexCoords
//
void Mesh::addTexCoord(const glm::vec2& _uv) {
    texcoords.push_back(_uv);
}

void Mesh::addTexCoord(const float* _data, int _n) {
    if (_n == 2)
        addTexCoord(_data[0], _data[1]);
}

void Mesh::addTexCoord(float _tX, float _tY) {
    addTexCoord( glm::vec2(_tX, _tY) );
}


void Mesh::addTexCoords(const float* _data, int _m, int _n) {
    for (int i = 0; i < _m; i++)
        addTexCoord(&_data[i*_n], _n);
}


bool Mesh::computeNormals() {
    if (getFaceType() != TRIANGLES) 
        return false;

    //The number of the vertices
    size_t nV = vertices.size();

    //The number of the triangles
    size_t nT = faceIndices.size() / 3;

    std::vector<glm::vec3> norm( nV ); //Array for the normals

    //Scan all the triangles. For each triangle add its
    //normal to norm's vectors of triangle's vertices
    for (size_t t=0; t < nT; t++) {

        //Get indices of the triangle t
        INDEX_TYPE i1 = faceIndices[ 3 * t ];
        INDEX_TYPE i2 = faceIndices[ 3 * t + 1 ];
        INDEX_TYPE i3 = faceIndices[ 3 * t + 2 ];

        //Get vertices of the triangle
        const glm::vec3 &v1 = vertices[ i1 ];
        const glm::vec3 &v2 = vertices[ i2 ];
        const glm::vec3 &v3 = vertices[ i3 ];

        //Compute the triangle's normal
        glm::vec3 dir = glm::normalize(glm::cross(v2-v1,v3-v1));

        //Accumulate it to norm array for i1, i2, i3
        norm[ i1 ] += dir;
        norm[ i2 ] += dir;
        norm[ i3 ] += dir;
    }

    //Normalize the normal's length and add it.
    normals.clear();
    for (size_t i=0; i < nV; i++)
        addNormal( glm::normalize(norm[i]) );

    return true;
}

void Mesh::invertWindingOrder() {
    if ( getFaceType() == TRIANGLES) {
        int tmp;
        for (size_t i = 0; i < faceIndices.size(); i += 3) {
            tmp = faceIndices[i+1];
            faceIndices[i+1] = faceIndices[i+2];
            faceIndices[i+2] = tmp;
        }
    }
}

void Mesh::invertNormals() {
    for (std::vector<glm::vec3>::iterator it = normals.begin(); it != normals.end(); ++it)
        *it *= -1.0f;
}

void Mesh::flatNormals() {
    if ( getFaceType() == TRIANGLES) {
        
        // get copy original mesh data
        size_t numIndices = faceIndices.size();
        std::vector<INDEX_TYPE> indices = faceIndices;
        std::vector<glm::vec3> verts = vertices;
        std::vector<glm::vec4> colors = colors;
        std::vector<glm::vec2> texCoords = texcoords;
        
        // remove all data to start from scratch
        clear();
        
        // add mesh data back, duplicating vertices and recalculating normals
        glm::vec3 normal;
        for (size_t i = 0; i < numIndices; i++) {
            size_t indexCurr = indices[i];
    
            if (i % 3 == 0) {
                INDEX_TYPE indexNext1 = indices[i + 1];
                INDEX_TYPE indexNext2 = indices[i + 2];
                glm::vec3 e1 = verts[indexCurr] - verts[indexNext1];
                glm::vec3 e2 = verts[indexNext2] - verts[indexNext1];
                normal = glm::normalize(glm::cross(e1, e2));
            }
    
            addFaceIndex(i);
            addNormal(normal);
            if (indexCurr < texCoords.size()) addTexCoord(texCoords[indexCurr]);
            if (indexCurr < verts.size()) addVertex(verts[indexCurr]);
            if (indexCurr < colors.size()) addColor(colors[indexCurr]);
        }
    }
}

// http://www.terathon.com/code/tangent.html
bool Mesh::computeTangents() {
    //The number of the vertices
    size_t nV = vertices.size();

    if (texcoords.size() != nV || 
        normals.size() != nV || 
        getFaceType() != TRIANGLES)
        return false;

    //The number of the triangles
    size_t nT = faceIndices.size() / 3;

    std::vector<glm::vec3> tan1( nV );
    std::vector<glm::vec3> tan2( nV );

    //Scan all the triangles. For each triangle add its
    //normal to norm's vectors of triangle's vertices
    for (size_t t = 0; t < nT; t++) {

        //Get indices of the triangle t
        INDEX_TYPE i1 = faceIndices[ 3 * t ];
        INDEX_TYPE i2 = faceIndices[ 3 * t + 1 ];
        INDEX_TYPE i3 = faceIndices[ 3 * t + 2 ];

        //Get vertices of the triangle
        const glm::vec3 &v1 = vertices[ i1 ];
        const glm::vec3 &v2 = vertices[ i2 ];
        const glm::vec3 &v3 = vertices[ i3 ];

        const glm::vec2 &w1 = texcoords[i1];
        const glm::vec2 &w2 = texcoords[i2];
        const glm::vec2 &w3 = texcoords[i3];

        float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;
        
        float s1 = w2.x - w1.x;
        float s2 = w3.x - w1.x;
        float t1 = w2.y - w1.y;
        float t2 = w3.y - w1.y;
        
        float r = 1.0f / (s1 * t2 - s2 * t1);
        glm::vec3 sdir( (t2 * x1 - t1 * x2) * r, 
                        (t2 * y1 - t1 * y2) * r, 
                        (t2 * z1 - t1 * z2) * r);
        glm::vec3 tdir( (s1 * x2 - s2 * x1) * r, 
                        (s1 * y2 - s2 * y1) * r, 
                        (s1 * z2 - s2 * z1) * r);
        
        tan1[i1] += sdir;
        tan1[i2] += sdir;
        tan1[i3] += sdir;
        
        tan2[i1] += tdir;
        tan2[i2] += tdir;
        tan2[i3] += tdir;
    }

    //Normalize the normal's length and add it.
    tangents.clear();
    for (size_t i = 0; i < nV; i++) {
        const glm::vec3 &n = normals[i];
        const glm::vec3 &t = tan1[i];
        
        // Gram-Schmidt orthogonalize
        glm::vec3 tangent = t - n * glm::dot(n, t);

        // Calculate handedness
        float hardedness = (glm::dot( glm::cross(n, t), tan2[i]) < 0.0f) ? -1.0f : 1.0f;

        addTangent(glm::vec4(tangent, hardedness));
    }

    return true;
}


// Indices
//
void Mesh::addIndices(const INDEX_TYPE* _data, int _m, int _n) {
    if (_n == 2) {
        for (int i = 0; i < _m; i++)
            addEdgeIndices(_data[i*_n], _data[i*_n+1]);
    }
    else if (_n == 3) {
        for (int i = 0; i < _m; i++)
            addTriangleIndices(_data[i*_n], _data[i*_n+1], _data[i*_n+2]);
    }
    else if (_n == 4) {
        for (int i = 0; i < _m; i++)
            addQuadIndices(_data[i*_n], _data[i*_n+1], _data[i*_n+2], _data[i*_n+3]);
    }
}

// FACE GROUPING
//
void Mesh::setFaceType(FaceType _mode, bool _compute) {
    faceMode = _mode;

    if (!haveVertices())
        return;

    if (_compute) {
        faceIndices.clear();
        
        if (_mode == TRIANGLES) {
            for (size_t j = 0; j < vertices.size(); j += 3)
                addTriangleIndices(j, j + 1, j + 2);
        }
    }
}

void Mesh::addFaceIndex(INDEX_TYPE _i) {
    faceIndices.push_back(_i);
}

void Mesh::addFaceIndices(const INDEX_TYPE* _array1D, int _n) {
    faceIndices.insert(faceIndices.end(),_array1D,_array1D+_n);
}

void Mesh::addTriangleIndices(INDEX_TYPE _index1, INDEX_TYPE _index2, INDEX_TYPE _index3) {
    addFaceIndex(_index1);
    addFaceIndex(_index2);
    addFaceIndex(_index3);
}

void Mesh::addTriangle(const Triangle& _tri) {
    INDEX_TYPE index = vertices.size();

    addVertex(_tri[0]);
    addVertex(_tri[1]);
    addVertex(_tri[2]);

    if (_tri.haveColors()) {
        addColor(_tri.getColor(0));
        addColor(_tri.getColor(1));
        addColor(_tri.getColor(2));
    }

    if (_tri.haveNormals()) {
        addNormal(_tri.getNormal(0));
        addNormal(_tri.getNormal(1));
        addNormal(_tri.getNormal(2));
    }
    else {
        addNormal(_tri.getNormal());
        addNormal(_tri.getNormal());
        addNormal(_tri.getNormal());
    }

    if (_tri.haveTexCoords()) {
        addTexCoord(_tri.getTexCoord(0));
        addTexCoord(_tri.getTexCoord(1));
        addTexCoord(_tri.getTexCoord(2));
    }

    addTriangleIndices(index, index+1, index+2);
}

void Mesh::addQuadIndices(INDEX_TYPE _index1, INDEX_TYPE _index2, INDEX_TYPE _index3, INDEX_TYPE _index4) {
    addFaceIndex(_index1);
    addFaceIndex(_index2);
    addFaceIndex(_index3);
    addFaceIndex(_index4);
}

std::vector<glm::ivec3> Mesh::getTrianglesIndices() const {
    std::vector<glm::ivec3> triangles;

    if (getFaceType() == TRIANGLES) {
        if (haveFaceIndices()) {
            for (size_t j = 0; j < faceIndices.size(); j += 3) {
                glm::ivec3 tri;
                for (int k = 0; k < 3; k++)
                    tri[k] = faceIndices[j+k];
                triangles.push_back(tri);
            }
        }
        else {
            for (size_t j = 0; j < vertices.size(); j += 3) {
                glm::ivec3 tri;
                for (int k = 0; k < 3; k++)
                    tri[k] = j+k;
                triangles.push_back(tri);
            }
        }
    }
    else if (getFaceType() == TRIANGLE_STRIP) {
        if (faceIndices.size() > 2) {
            int a = int(faceIndices[0]);
            int b = int(faceIndices[1]);
            int c;
            bool CCW = true;
            for (size_t j = 2; j < faceIndices.size(); j++) {
                c = int(faceIndices[j]);
                // Account for degenerate triangles
                if (a != b && b != c && c != a) {
                    if (CCW) triangles.push_back(glm::ivec3(a, c, b));
                    else triangles.push_back(glm::ivec3(a, b, c));
                }
                a = b;
                b = c;
                CCW = !CCW;
            }
        }
        else {
            int a = 0;
            int b = 1;
            bool CCW = true;
            for (size_t c = 2; c < vertices.size(); c++) {
                if (CCW) triangles.push_back(glm::ivec3(a, c, b));
                else triangles.push_back(glm::ivec3(a, b, c));
                a = b;
                b = c;
                CCW = !CCW;
            }
        }
        
    }
    else if (getFaceType() == QUAD) {
        if (haveFaceIndices()) {
            for (size_t j = 0; j < faceIndices.size(); j += 4) {
                triangles.push_back(glm::ivec3(faceIndices[j], faceIndices[j+2], faceIndices[j+1]));
                triangles.push_back(glm::ivec3(faceIndices[j+2], faceIndices[j], faceIndices[j+3]));
            }
        }
        else {
            for (size_t j = 0; j < vertices.size(); j += 4) {
                std::cout << "j " << j << std::endl;
                triangles.push_back(glm::ivec3(j, j+2, j+1));
                triangles.push_back(glm::ivec3(j+2, j, j+3));
            }
        }
    }
    else {
        //  TODO
        //
        std::cout << "ERROR: getTriangles(): Mesh only add TRIANGLES for NOW !!" << std::endl;
    }

    return triangles;
}

void Mesh::addTriangles(const Triangle* _array1D, int _n) {
    for (int i = 0; i < _n; i++)
        addTriangle(_array1D[i]);
}

std::vector<Triangle> Mesh::getTriangles() const {
    std::vector<glm::ivec3> triIndices = getTrianglesIndices();
    std::vector<Triangle> triangles;

    for (std::vector<glm::ivec3>::iterator it = triIndices.begin(); it != triIndices.end(); ++it) {
        Triangle tri = Triangle(vertices[it->x], vertices[it->y], vertices[it->z]);
        if (haveColors()) tri.setColors(colors[it->x], colors[it->y], colors[it->z]);
        if (haveNormals()) tri.setNormals(normals[it->x], normals[it->y], normals[it->z]);
        if (haveTexCoords()) tri.setTexCoords(texcoords[it->x], texcoords[it->y], texcoords[it->z]);
        triangles.push_back( tri );
    }

    return triangles;
}


// EDGE GROUPING
//
void Mesh::addEdgeIndex(INDEX_TYPE _i) {
    edgeIndices.push_back(_i);
}

void Mesh::addEdgeIndices(const INDEX_TYPE* _array1D, int _n) {
    edgeIndices.insert(edgeIndices.end(), _array1D, _array1D+_n);
}

void Mesh::addEdgeIndices( INDEX_TYPE _index1, INDEX_TYPE _index2 ) {
    addEdgeIndex(_index1);
    addEdgeIndex(_index2);
}

void Mesh::addEdge(const Line& _line) {
    // TODO
    INDEX_TYPE index = vertices.size();

    addVertex(_line[0]);
    addVertex(_line[1]);

    addEdgeIndices( index+0, index+1);
}

void Mesh::addEdge(const Ray& _ray) {
    // TODO
    INDEX_TYPE index = vertices.size();

    addVertex(_ray.getOrigin());
    addVertex(_ray.getOrigin() + _ray.getDirection());

    addEdgeIndices( index+0, index+1);
}

void Mesh::setEdgeType(EdgeType _mode, bool _compute) {
    edgeMode = _mode;

    if (!haveVertices())
        return;

    if (_compute) {
        edgeIndices.clear();

        if (_mode == LINES) {
            for (size_t j = 0; j < vertices.size(); j += 2)
                addEdgeIndices(j, j + 1);
        }
    }
}

std::vector<glm::ivec2> Mesh::getLinesIndices() const {
    std::vector<glm::ivec2> lines;

    if (getEdgeType() == LINES) {
        if (haveFaceIndices()) {
            for (size_t j = 0; j < edgeIndices.size(); j += 2) {
                glm::ivec2 line;
                for (int k = 0; k < 2; k++)
                    line[k] = edgeIndices[j+k];
                lines.push_back(line);
            }
        }
        else {
            for (size_t j = 0; j < vertices.size(); j += 2) {
                glm::ivec2 line;
                for (int k = 0; k < 2; k++)
                    line[k] = j+k;
                lines.push_back(line);
            }
        }
    }

    return lines;
}

std::vector<Line> Mesh::getLinesEdges() const {
    std::vector<glm::ivec2> linesIndices;
    std::vector<Line> lines;

    for (std::vector<glm::ivec2>::iterator it = linesIndices.begin(); it != linesIndices.end(); ++it)
        lines.push_back( Line(vertices[it->x], vertices[it->y]) );

    return lines;
}


void Mesh::mergeDuplicateVertices() {

    std::vector<glm::vec3> verts = vertices;
    std::vector<INDEX_TYPE> indices = faceIndices;

    //get indexes to share single point - TODO: try j < i
    for (INDEX_TYPE i = 0; i < indices.size(); i++) {
        for (INDEX_TYPE j = 0; j < indices.size(); j++ ) {
            if (i==j) continue;

            INDEX_TYPE i1 = indices[i];
            INDEX_TYPE i2 = indices[j];
            const glm::vec3& v1 = verts[ i1 ];
            const glm::vec3& v2 = verts[ i2 ];

            if ( v1 == v2 && i1 != i2) {
                indices[j] = i1;
                break;
            }
        }
    }

    //indices array now has list of unique points we need
    //but we need to delete the old points we're not using and that means the index values will change
    //so we are going to create a new list of points and new indexes - we will use a map to map old index values to the new ones
    std::vector<glm::vec3> newPoints;
    std::vector<INDEX_TYPE> newIndexes;
    std::map<INDEX_TYPE, bool> ptCreated;
    std::map<INDEX_TYPE, INDEX_TYPE> oldIndexNewIndex;

    std::vector<glm::vec4> newColors;
    std::vector<glm::vec4>& colors = colors;
    std::vector<glm::vec2> newTCoords;
    std::vector<glm::vec2>& tcoords = texcoords;
    std::vector<glm::vec3> newNormals;
    std::vector<glm::vec3>& normals =  normals;

    for (INDEX_TYPE i = 0; i < indices.size(); i++)
        ptCreated[i] = false;

    for (INDEX_TYPE i = 0; i < indices.size(); i++){
        INDEX_TYPE index = indices[i];
        const glm::vec3& p = verts[ index ];

        if ( ptCreated[index] == false ){
            oldIndexNewIndex[index] = newPoints.size();
            newPoints.push_back( p );
            if (haveColors())
                newColors.push_back(colors[index]);
            
            if (haveTexCoords())
                newTCoords.push_back(tcoords[index]);
            
            if (haveNormals())
                newNormals.push_back(normals[index]);

            ptCreated[index] = true;
        }

        //ofLogNotice("ofMesh") << "[" << i << "]: old " << index << " --> " << oldIndexNewIndex[index];
        newIndexes.push_back( oldIndexNewIndex[index] );
    }

    verts.clear();
    verts = newPoints;

    indices.clear();
    indices = newIndexes;

    clearFaceIndices();
    addFaceIndices(&indices[0], indices.size());
    clearVertices();
    addVertices( &verts[0].x, verts.size(), 3);

    if (haveColors()) {
        clearColors();
        addColors( &newColors[0].x, newColors.size(), 4);
    }

    if (haveTexCoords()) {
        clearTexCoords();
        addTexCoords( &newTCoords[0].x, newTCoords.size(), 2);
    }

    if (haveNormals()) {
        clearNormals();
        addNormals( &newNormals[0].x, newNormals.size(), 3);
    }

}
