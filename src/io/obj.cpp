#include "hilma/io/obj.h"

#include <stdio.h>
#include <iostream>
#include <limits>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cassert>

#define TINYOBJLOADER_IMPLEMENTATION
#include "hilma/io/tiny_obj_loader.h"

#ifndef HILMA_LINE_MAX
#define HILMA_LINE_MAX 2048
#endif

#include "hilma/fs.h"
#include "hilma/text.h"

namespace hilma {

Material InitMaterial (const tinyobj::material_t& _material) {
    Material mat = Material( toLower( toUnderscore( purifyString( _material.name ) ) ) );

    if (_material.diffuse_texname.size() > 0)
        mat.set("diffuse", _material.diffuse_texname);
    else
        mat.set("diffuse", &_material.diffuse[0], 3);

    if (_material.specular_texname.size() > 0)
        mat.set("specular", _material.specular_texname);
    else
        mat.set("specular", &_material.specular[0], 3);

    if (_material.emissive_texname.size() > 0)
        mat.set("emissive", _material.emissive_texname);
    else
        mat.set("emissive", &_material.emission[0], 3);

    if (_material.roughness_texname.size() > 0)
        mat.set("roughness", _material.roughness_texname);
    else if (_material.roughness > 0.0f)
        mat.set("roughness", _material.roughness);

    if (_material.metallic_texname.size() > 0)
        mat.set("metallic", _material.metallic_texname);
    else if (_material.roughness > 0.0f)
        mat.set("metallic", _material.metallic);

    // if (_material.sheen_texname.size() > 0)
    //     mat.set("sheen", _material.sheen_texname);
    // else
    //     mat.set("sheen", _material.sheen);

    // if (_material.normal_texname.size() > 0)
    //     mat.set("normal", _material.normal_texname);

    // if (_material.bump_texname.size() > 0)
    //     mat.set("bump", _material.bump_texname);

    // if (_material.alpha_texname.size() > 0)
    //     mat.set("alpha", _material.alpha_texname);

    // if (_material.displacement_texname.size() > 0)
    //     mat.set("displacement", _material.displacement_texname);
    
    // mat.shininess = _material.shininess;

    // mat.anisotropy = _material.anisotropy;
    // mat.anisotropyRotation = _material.anisotropy_rotation;

    // mat.clearcoatRoughness = _material.clearcoat_roughness;
    // mat.clearcoatThickness = _material.clearcoat_thickness;

    // mat.ior = _material.ior;
    // mat.dissolve = _material.dissolve;
    // mat.transmittance = glm::vec3(_material.transmittance[0], _material.transmittance[1], _material.transmittance[2]);
    // mat.reflectionMapName = _material.reflection_texname;
    // mat.specularHighlightMapName = _material.specular_highlight_texname;

    mat.illuminationModel = _material.illum;
    return mat;
}

bool saveMaterials(const std::string& _filename, const Mesh& _mesh) {
    std::vector<std::string> materials = _mesh.getMaterialsNames();
    if (materials.size() == 0)
        return true;

    FILE * mtl_file = fopen(_filename.c_str(), "w");
    if (NULL == mtl_file) {
        printf("IOError: %s could not be opened for writing...", _filename.c_str());
        return false;
    }

    fprintf(mtl_file, "# Material Count: %i \n", (int)materials.size());

    std::vector<std::string> mat_value_obj = {           "Ns",               "Ni",       "d",        "Pr",       "Pm",    "Ps",                  "Pc",                 "Pcr",      "aniso",              "anisor"};
    std::vector<std::string> mat_value_name= { "specular_exp",  "optical_density", "opacity", "roughness", "metallic", "sheen", "clearcoat_thickness", "clearcoat_roughness", "anisotropy", "anisotropy rotation"};

    std::vector<std::string> mat_color_obj = {      "Ka",      "Kd",       "Ks",       "Ke" };
    std::vector<std::string> mat_color_name= { "ambient", "diffuse", "specular", "emissive" };
    
    std::vector<std::string> mat_tex_obj   = { "bump",               "disp",      "norm" };
    std::vector<std::string> mat_tex_name  = { "bumpmap", "displacementmap", "normalmap" };

    for (size_t i = 0; i < materials.size(); i++) {
        fprintf(mtl_file, "\nnewmtl %s\n", materials[i].c_str());
        MaterialConstPtr mat = _mesh.getMaterial(materials[i]);

        for (size_t j = 0; j < mat_value_obj.size(); j++)
            if (mat->haveProperty(mat_value_name[j])) {
                if (mat->getImagePath(mat_value_name[j]).size() > 0) 
                    fprintf(mtl_file, "map_%s %s\n", mat_value_obj[j].c_str(), mat->getImagePath(mat_value_name[j]).c_str() );
                else
                    fprintf(mtl_file, "%s %.3f\n", mat_value_obj[j].c_str(), mat->getValue(mat_value_name[j]) );
            }

        for (size_t j = 0; j < mat_color_obj.size(); j++)
            if (mat->haveProperty(mat_color_name[j])) {
                if (mat->getImagePath(mat_color_name[j]).size() > 0)
                    fprintf(mtl_file, "map_%s %s\n", mat_color_obj[j].c_str(), mat->getImagePath(mat_color_name[j]).c_str() );
                else {
                    glm::vec3 color = mat->getColor(mat_color_name[j]);
                    fprintf(mtl_file, "%s %.3f %.3f %.3f\n", mat_color_obj[j].c_str(), color.r, color.g, color.b );
                }
            }

        for (size_t j = 0; j < mat_tex_obj.size(); j++)
            if (mat->getImagePath(mat_tex_name[j]).size() > 0) 
                fprintf(mtl_file, "map_%s %s\n", mat_tex_obj[j].c_str(), mat->getImagePath(mat_tex_name[j]).c_str() );

        fprintf(mtl_file, "illum %i\n", mat->illuminationModel);
    }

    fclose(mtl_file);

    return true;
}

glm::vec3 getVertex(const tinyobj::attrib_t& _attrib, int _index) {
    return glm::vec3(   _attrib.vertices[3 * _index + 0],
                        _attrib.vertices[3 * _index + 1],
                        _attrib.vertices[3 * _index + 2]);
}

glm::vec4 getColor(const tinyobj::attrib_t& _attrib, int _index) {
    return glm::vec4(   _attrib.colors[3 * _index + 0],
                        _attrib.colors[3 * _index + 1],
                        _attrib.colors[3 * _index + 2],
                        1.0);
}

glm::vec3 getNormal(const tinyobj::attrib_t& _attrib, int _index) {
    return glm::vec3(   _attrib.normals[3 * _index + 0],
                        _attrib.normals[3 * _index + 1],
                        _attrib.normals[3 * _index + 2]);
}

glm::vec2 getTexCoords(const tinyobj::attrib_t& _attrib, int _index) {
    return glm::vec2(   _attrib.texcoords[2 * _index], 
                        1.0f - _attrib.texcoords[2 * _index + 1]);
}

void calcNormal(const glm::vec3& _v0, const glm::vec3& _v1, const glm::vec3& _v2, glm::vec3& _N) {
    glm::vec3 v10 = _v1 - _v0;
    glm::vec3 v20 = _v2 - _v0;

    _N.x = v20.x * v10.z - v20.z * v10.y;
    _N.y = v20.z * v10.x - v20.x * v10.z;
    _N.z = v20.x * v10.y - v20.y * v10.x;
    
    _N = glm::normalize(_N);
}

// Check if `mesh_t` contains smoothing group id.
bool hasSmoothingGroup(const tinyobj::shape_t& shape) {
    for (size_t i = 0; i < shape.mesh.smoothing_group_ids.size(); i++)
        if (shape.mesh.smoothing_group_ids[i] > 0)
            return true;
        
    return false;
}

void computeSmoothingNormals(const tinyobj::attrib_t& _attrib, const tinyobj::shape_t& _shape, std::map<int, glm::vec3>& smoothVertexNormals) {
    smoothVertexNormals.clear();

    std::map<int, glm::vec3>::iterator iter;

    for (size_t f = 0; f < _shape.mesh.indices.size() / 3; f++) {
        // Get the three indexes of the face (all faces are triangular)
        tinyobj::index_t idx0 = _shape.mesh.indices[3 * f + 0];
        tinyobj::index_t idx1 = _shape.mesh.indices[3 * f + 1];
        tinyobj::index_t idx2 = _shape.mesh.indices[3 * f + 2];

        // Get the three vertex indexes and coordinates
        int vi[3];      // indexes
        vi[0] = idx0.vertex_index;
        vi[1] = idx1.vertex_index;
        vi[2] = idx2.vertex_index;

        glm::vec3 v[3];  // coordinates
        for (size_t i = 0; i < 3; i++)
            v[i] = getVertex(_attrib, vi[i]);

        // Compute the normal of the face
        glm::vec3 normal;
        calcNormal(v[0], v[1], v[2], normal);

        // Add the normal to the three vertexes
        for (size_t i = 0; i < 3; ++i) {
            iter = smoothVertexNormals.find(vi[i]);
            // add
            if (iter != smoothVertexNormals.end())
                iter->second += normal;
            else
                smoothVertexNormals[vi[i]] = normal;
        }
    }  // f

    // Normalize the normals, that is, make them unit vectors
    for (iter = smoothVertexNormals.begin(); iter != smoothVertexNormals.end(); iter++) {
        iter->second = glm::normalize(iter->second);
    }
}

bool loadObj( const std::string& _filename, Mesh& _mesh ) {

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;
    std::string base_dir = getBaseDir(_filename.c_str());
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, _filename.c_str(), base_dir.c_str());

    if (!warn.empty()) std::cout << "WARN: " << warn << std::endl;
    
    if (!err.empty()) std::cerr << err << std::endl;
    
    if (!ret) {
        std::cerr << "Failed to load " << _filename.c_str() << std::endl;
        return false;
    }

    for (size_t s = 0; s < shapes.size(); s++) {
        
        std::string name = shapes[s].name;
        if (name.empty())
            name = toString(s);
        _mesh.name = name;

        // Check for smoothing group and compute smoothing normals
        std::map<int, glm::vec3> smoothVertexNormals;
        if (hasSmoothingGroup(shapes[s]) > 0)
            computeSmoothingNormals(attrib, shapes[s], smoothVertexNormals);

        // std::map<INDEX_TYPE, tinyobj::index_t> unique_indices;
        // std::map<INDEX_TYPE, tinyobj::index_t>::iterator iter;
        
        int mi = -1;
        INDEX_TYPE iCounter = 0;
        for (size_t i = 0; i < shapes[s].mesh.indices.size(); i++) {

            tinyobj::index_t index = shapes[s].mesh.indices[i];
            int vi = index.vertex_index;
            int ni = index.normal_index;
            int ti = index.texcoord_index;

            // Associate w material with face
            int f = (int)floor(i/3);
            if (shapes[s].mesh.material_ids.size() > 0) {
                int material_index = shapes[s].mesh.material_ids[f];

                // but only when change
                if (mi != material_index) {
                    mi = material_index;
                    _mesh.addMaterial( InitMaterial( materials[mi] ) );
                }
            }

            // bool reuse = false;
            // iter = unique_indices.find(vi);

            // // if already exist 
            // if (iter != unique_indices.end())
            //     // and have the same attributes
            //     if ((iter->second.normal_index == ni) &&
            //         (iter->second.texcoord_index == ti) )
            //         reuse = true;

            // // Re use the vertex
            // if (reuse)
            //     _mesh.addFaceIndex( (INDEX_TYPE)iter->second.vertex_index );

            // // Other wise create a new one
            // else 
            {
                // unique_indices[vi].vertex_index = iCounter;
                // unique_indices[vi].normal_index = ni;
                // unique_indices[vi].texcoord_index = ti;
                
                _mesh.addVertex( getVertex(attrib, vi) );

                if (attrib.colors.size() > 0)
                    _mesh.addColor( getColor(attrib, vi) );

                // If there is normals add them
                if (attrib.normals.size() > 0)
                    _mesh.addNormal( getNormal(attrib, ni) );

                else if (smoothVertexNormals.size() > 0)
                    if ( smoothVertexNormals.find(vi) != smoothVertexNormals.end() )
                        _mesh.addNormal( smoothVertexNormals.at(vi) );

                // If there is texcoords add them
                if (attrib.texcoords.size() > 0)
                    _mesh.addTexCoord( getTexCoords(attrib, ti) );

                _mesh.addFaceIndex( iCounter );
                iCounter++;
            }
            
        }

    }

    return true;
}

Mesh loadObj( const std::string& _filename) {
    Mesh mesh;
    loadObj(_filename, mesh);
    return mesh;
}

bool saveObj( const std::string& _filename, const Mesh& _mesh ) {

    FILE * obj_file = fopen(_filename.c_str(), "w");
    if (NULL == obj_file) {
        printf("IOError: %s could not be opened for writing...", _filename.c_str());
        return false;
    }

    saveMaterials(_filename.substr(0,_filename.size()-3) + "mtl", _mesh);
    
    // https://github.com/libigl/libigl/blob/master/include/igl/writeOBJ.cpp
    fprintf(obj_file,"# generated with Hilma by Patricio Gonzalez Vivo\n");
    fprintf(obj_file,"o %s\n", _mesh.getName().c_str());

    // TODO:
    //      - MATERIALS

    // Loop over V
    for(size_t i = 0; i < _mesh.vertices.size(); i++)
        fprintf(obj_file,"v %0.17g %0.17g %0.17g\n", _mesh.vertices[i].x, _mesh.vertices[i].y, _mesh.vertices[i].z); 
    // fprintf(obj_file,"\n");

    bool write_normals = _mesh.haveNormals();
    if (write_normals) {
        for (size_t i = 0; i < _mesh.normals.size(); i++)
            fprintf(obj_file,"vn %0.17g %0.17g %0.17g\n", _mesh.normals[i].x, _mesh.normals[i].y, _mesh.normals[i].z);
        // fprintf(obj_file,"\n");
    }

    bool write_texture_coords = _mesh.haveTexCoords();
    if (write_texture_coords) {
        for (size_t i = 0; i < _mesh.texcoords.size(); i++)
            fprintf(obj_file, "vt %0.17g %0.17g\n", _mesh.texcoords[i].x, _mesh.texcoords[i].y);
        // fprintf(obj_file,"\n");
    }

    bool materials = _mesh.haveMaterials();
    if (_mesh.haveFaceIndices()) {
        std::vector<glm::ivec3> faces = _mesh.getTrianglesIndices();
        std::string last_material = "";
        for (size_t i = 0; i < faces.size(); i++) {

            std::string matname = _mesh.getMaterialForFaceIndex( faces[i][0] )->name;
            if (matname != last_material) {
                if (materials) {
                    fprintf(obj_file, "usemtl %s\n", matname.c_str() );
                    last_material = matname;
                }
            }

            fprintf(obj_file,"f");

            for(int j = 0; j < 3; j++) {
                fprintf(obj_file," %u", faces[i][j] + 1);

                if(write_texture_coords)
                    fprintf(obj_file,"/%u", faces[i][j] + 1);
            
                if(write_normals) {
                    if (write_texture_coords)
                        fprintf(obj_file,"/%u", faces[i][j] + 1);
                    else
                        fprintf(obj_file,"//%u", faces[i][j] + 1);
                }
            }

            fprintf(obj_file,"\n");
        }
    } fclose(obj_file);
   

    return true;
};


}

