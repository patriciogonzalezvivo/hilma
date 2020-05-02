#include <iostream>

#include "hilma/types/Mesh.h"

using namespace hilma;

Mesh::Mesh() : m_mode(TRIANGLES) {

}

Mesh::Mesh(const Mesh& _mother): m_mode(_mother.getMode()) {
    append(_mother);
}

Mesh::~Mesh() {

}

void Mesh::clear() {
    if (!m_vertices.empty()) m_vertices.clear();
    
    if (!m_colors.empty()) m_colors.clear();
    if (!m_normals.empty()) m_normals.clear();
    if (!m_texcoords.empty()) m_texcoords.clear();
    if (!m_tangents.empty()) m_tangents.clear();
    if (!m_indices.empty()) m_indices.clear();
    // if (!m_indices_normals.empty()) m_indices_normals.clear();
    // if (!m_indices_texcoords.empty()) m_indices_texcoords.clear();
    // if (!m_edge_colors.empty()) m_edge_colors.clear();
    // if (!m_edge_indices.empty()) m_edge_indices.clear();
}

void Mesh::append(const Mesh& _mesh) {
    int indexOffset = (int)m_vertices.size();

	if (_mesh.hasVertices())
		m_vertices.insert(m_vertices.end(), _mesh.m_vertices.begin(), _mesh.m_vertices.end());
	
    if (_mesh.hasTexCoords())
		m_texcoords.insert(m_texcoords.end(), _mesh.m_texcoords.begin(), _mesh.m_texcoords.end());
	
    if (_mesh.hasColors())
		m_colors.insert(m_colors.end(),_mesh.m_colors.begin(),_mesh.m_colors.end());
	
    if (_mesh.hasNormals())
		m_normals.insert(m_normals.end(),_mesh.m_normals.begin(),_mesh.m_normals.end());

    if (_mesh.getMode() != m_mode) {
        std::cout << "INCOMPATIBLE DRAW MODES" << std::endl;
        return;
    }

	if (_mesh.hasIndices())
        for (uint32_t i = 0; i < _mesh.m_indices.size(); i++)
            addIndex(indexOffset+_mesh.m_indices[i]);
}


// Vertices
//
void Mesh::addVertex(const glm::vec3& _point) {
   m_vertices.push_back(_point);
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

// void Mesh::addVertices(const float* _data, int _n) {
    // size_t tail = m_vertices.size();
    // m_vertices.resize(tail+_n/3);
    // std::memcpy(&m_vertices[tail], _data, sizeof(float) * _n);
// }

void Mesh::addVertices(const float* _data, int _m, int _n) {
    for (int i = 0; i < _m; i++)
        addVertex(&_data[i*_n], _n);
}


// Color
//
void Mesh::setColor(const glm::vec4& _color) {
    m_colors.clear();
    for (uint32_t i = 0; i < m_vertices.size(); i++)
        m_colors.push_back(_color);
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
    m_colors.push_back(_color);
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
    m_normals.push_back(_normal);
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
    m_tangents.push_back(_tangent);
}

void Mesh::addTangent(const float* _data, int _n) {
    if (_n == 4)
        addTangent(_data[0], _data[1], _data[2], _data[3]);
}

void Mesh::addTangent(float _x, float _y, float _z, float _w) {
    m_tangents.push_back( glm::vec4(_x, _y, _z, _w) );
}


// TexCoords
//
void Mesh::addTexCoord(const glm::vec2& _uv) {
    m_texcoords.push_back(_uv);
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


// Indices
//
void Mesh::addIndex(int _i) {
    m_indices.push_back(_i);
}

void Mesh::addIndices(const int* _data, int _n) {
    m_indices.insert(m_indices.end(),_data,_data+_n);
}


// Faces Grouping
//
void Mesh::addFaces(const int* _data, int _m, int _n) {
    if (_n == 2) {
        for (int i = 0; i < _m; i++)
            addLine(_data[i*_n], _data[i*_n+1]);
    }
    else if (_n == 3) {
        for (int i = 0; i < _m; i++)
            addTriangle(_data[i*_n], _data[i*_n+1], _data[i*_n+2]);
    }
}
void Mesh::addLine( int _index1, int _index2 ){
    addIndex(_index1);
    addIndex(_index2);
}

void Mesh::addTriangle(int _index1, int _index2, int _index3) {
    addIndex(_index1);
    addIndex(_index2);
    addIndex(_index3);
}

std::vector<glm::ivec3> Mesh::getTriangles() const {
    std::vector<glm::ivec3> faces;

    if (getMode() == TRIANGLES) {
        if (hasIndices()) {
            for (size_t j = 0; j < m_indices.size(); j += 3) {
                glm::ivec3 tri;
                for (int k = 0; k < 3; k++)
                    tri[k] = m_indices[j+k];
                faces.push_back(tri);
            }
        }
        else {
            for ( size_t j = 0; j < m_vertices.size(); j += 3) {
                glm::ivec3 tri;
                for (int k = 0; k < 3; k++)
                    tri[k] = j+k;
                faces.push_back(tri);
            }
        }
    }
    else {
        //  TODO
        //
        std::cout << "ERROR: getTriangles(): Mesh only add TRIANGLES for NOW !!" << std::endl;
    }

    return faces;
}

void Mesh::setMode(PrimitiveMode _mode, bool _compute) {
    m_mode = _mode;

    if (!hasVertices())
        return;

    if (_compute) {
        m_indices.clear();

        if (_mode == LINES) {
            for ( size_t j = 0; j < m_vertices.size(); j += 2)
                addLine(j, j + 1);
        }
        else if (_mode == TRIANGLES) {
            for ( size_t j = 0; j < m_vertices.size(); j += 3)
                addTriangle(j, j + 1, j + 2);
        }
        // else if (_mode == QUAD) {
        //     for ( size_t j = 0; j < m_vertices.size(); j += 3)
        //         addQuad(j, j + 1, j + 2, j + 3);
        // }
    }
}

bool Mesh::computeNormals() {
    if (getMode() != TRIANGLES) 
        return false;

    //The number of the m_vertices
    int nV = m_vertices.size();

    //The number of the triangles
    int nT = m_indices.size() / 3;

    std::vector<glm::vec3> norm( nV ); //Array for the m_normals

    //Scan all the triangles. For each triangle add its
    //normal to norm's vectors of triangle's m_vertices
    for (int t=0; t<nT; t++) {

        //Get m_indices of the triangle t
        int i1 = m_indices[ 3 * t ];
        int i2 = m_indices[ 3 * t + 1 ];
        int i3 = m_indices[ 3 * t + 2 ];

        //Get m_vertices of the triangle
        const glm::vec3 &v1 = m_vertices[ i1 ];
        const glm::vec3 &v2 = m_vertices[ i2 ];
        const glm::vec3 &v3 = m_vertices[ i3 ];

        //Compute the triangle's normal
        glm::vec3 dir = glm::normalize(glm::cross(v2-v1,v3-v1));

        //Accumulate it to norm array for i1, i2, i3
        norm[ i1 ] += dir;
        norm[ i2 ] += dir;
        norm[ i3 ] += dir;
    }

    //Normalize the normal's length and add it.
    m_normals.clear();
    for (int i=0; i<nV; i++)
        addNormal( glm::normalize(norm[i]) );

    return true;
}

void Mesh::invertWindingOrder() {
    if( getMode() == TRIANGLES) {
        int tmp;
        for (size_t i = 0; i < m_indices.size(); i += 3) {
            tmp = m_indices[i+1];
            m_indices[i+1] = m_indices[i+2];
            m_indices[i+2] = tmp;
        }
    }
}

void Mesh::invertNormals() {
    for (std::vector<glm::vec3>::iterator it = m_normals.begin(); it != m_normals.end(); ++it)
        *it *= -1.0f;
}

void Mesh::flatNormals() {
    if( getMode() == TRIANGLES) {
        
        // get copy original mesh data
        size_t numIndices = m_indices.size();
        std::vector<int> indices = m_indices;
        std::vector<glm::vec3> verts = m_vertices;
        std::vector<glm::vec4> colors = m_colors;
        std::vector<glm::vec2> texCoords = m_texcoords;
        
        // remove all data to start from scratch
        clear();
        
        // add mesh data back, duplicating vertices and recalculating normals
        glm::vec3 normal;
        for (size_t i = 0; i < numIndices; i++) {
            size_t indexCurr = indices[i];
    
            if (i % 3 == 0) {
                int indexNext1 = indices[i + 1];
                int indexNext2 = indices[i + 2];
                auto e1 = verts[indexCurr] - verts[indexNext1];
                auto e2 = verts[indexNext2] - verts[indexNext1];
                normal = glm::normalize(glm::cross(e1, e2));
            }
    
            addIndex(i);
            addNormal(normal);
            if(indexCurr < texCoords.size()) addTexCoord(texCoords[indexCurr]);
            if(indexCurr < verts.size()) addVertex(verts[indexCurr]);
            if(indexCurr < colors.size()) addColor(colors[indexCurr]);
        }
    }
}

// http://www.terathon.com/code/tangent.html
bool Mesh::computeTangents() {
    //The number of the m_vertices
    size_t nV = m_vertices.size();

    if (m_texcoords.size() != nV || 
        m_normals.size() != nV || 
        getMode() != TRIANGLES)
        return false;

    //The number of the triangles
    size_t nT = m_indices.size() / 3;

    std::vector<glm::vec3> tan1( nV );
    std::vector<glm::vec3> tan2( nV );

    //Scan all the triangles. For each triangle add its
    //normal to norm's vectors of triangle's m_vertices
    for (size_t t = 0; t < nT; t++) {

        //Get m_indices of the triangle t
        int i1 = m_indices[ 3 * t ];
        int i2 = m_indices[ 3 * t + 1 ];
        int i3 = m_indices[ 3 * t + 2 ];

        //Get m_vertices of the triangle
        const glm::vec3 &v1 = m_vertices[ i1 ];
        const glm::vec3 &v2 = m_vertices[ i2 ];
        const glm::vec3 &v3 = m_vertices[ i3 ];

        const glm::vec2 &w1 = m_texcoords[i1];
        const glm::vec2 &w2 = m_texcoords[i2];
        const glm::vec2 &w3 = m_texcoords[i3];

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
    m_tangents.clear();
    for (size_t i = 0; i < nV; i++) {
        const glm::vec3 &n = m_normals[i];
        const glm::vec3 &t = tan1[i];
        
        // Gram-Schmidt orthogonalize
        glm::vec3 tangent = t - n * glm::dot(n, t);

        // Calculate handedness
        float hardedness = (glm::dot( glm::cross(n, t), tan2[i]) < 0.0f) ? -1.0f : 1.0f;

        addTangent(glm::vec4(tangent, hardedness));
    }

    return true;
}
