#pragma once

#include "hilma/accel/BoundingBox.h"

#include "hilma/types/Triangle.h"
#include "hilma/types/Ray.h"

#include <memory>

namespace hilma {

class BVH : public BoundingBox {
public:
    BVH();
    BVH( const std::vector<Triangle>& _elements, int _splitAxis = -1 );

    virtual void load( const std::vector<Triangle>& _elements, int _splitAxis = -1 );
    virtual std::shared_ptr<BVH> hit(const Ray& _ray, float& _minDistance, float& _maxDistance);
    virtual std::shared_ptr<BVH> closest(const glm::vec3& _point);

    virtual void split(int _axis = -1);
    virtual void clear();

    virtual int  getTotal();

    std::vector<Triangle>       elements;

    std::shared_ptr<const BVH>  parent;
    std::shared_ptr<BVH>        left;
    std::shared_ptr<BVH>        right;

    bool                        leaf;
};

}