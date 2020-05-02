#include "hilma/compute.h"

namespace hilma {

BoundingBox boundingBox(const std::vector<glm::vec3>& _points ) {
    BoundingBox bbox;

    for(std::vector<glm::vec3>::const_iterator it = _points.begin(); it != _points.end(); ++it)
        bbox.expand(*it);
    
    return bbox;
}

}

