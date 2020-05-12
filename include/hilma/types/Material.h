#pragma once

#include <vector>
#include <string>

#include "glm/glm.hpp"

namespace hilma {

class Material {
public:

    Material();
    Material(const std::string& _name);
    virtual ~Material();

    std::string name;
    
    std::string ambient_map;        // map_Ka
    glm::vec3   ambient;            // Ka

    std::string diffuse_map;        // map_Kd
    glm::vec3   diffuse;            // Kd
    
    std::string specular_map;       // map_Ks
    glm::vec3   specular;           // Ks
    
    std::string emissive_map;       // map_Ke
    glm::vec3   emissive;           // Ke

    std::string roughness_map;
    float       roughness;

    std::string metallic_map;
    float       metallic;
    
    std::string normal_map;
    std::string bump_map;           // map_bump
    std::string displacement_map;
    
    float       opacity;            // d
    std::string opacity_map;        // map_d

    float       sheen;
    std::string sheen_map;

    float       shininess;
    float       anisotropy;
    float       anisotropy_rotation;
    float       clearcoat_roughness;
    float       clearcoat_thickness;
    float       ior;

    float       dissolve;
    float       optical_density;    // Ni
    glm::vec3   transmittance;
    std::string reflection_map;
    std::string specular_highlight_map;

    int         illumination_model; // illum
};

}