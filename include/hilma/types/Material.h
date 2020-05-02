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
    
    std::string specular_exp_map;   // map_Ns
    float       specular_exp;       // Ns
    
    std::string emissive_map;       // map_Ke
    glm::vec3   emissive;           // Ke
    
    std::string opacity_map;        // map_d
    std::string bump_map;           // map_bump

    float       optical_density;    // Ni
    float       opacity;            // d

    int         illumination_model; // illum

    // TODOs:
    //      
    // std::string displacement_map;    // disp
    // std::string reflection_map;      // refl
};

}