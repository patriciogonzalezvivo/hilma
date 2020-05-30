#include "hilma/accel/BVH.h"

#include "hilma/ops/intersection.h"

#include <algorithm>

namespace hilma {

BVH::BVH() : parent(nullptr), left(nullptr), right(nullptr), leaf(false) {

}

BVH::BVH( const std::vector<Triangle>& _elements, int _splitAxis ) {
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    load(_elements, _splitAxis);
}

void BVH::load( const std::vector<Triangle>& _elements, int _splitAxis ) {
    elements = _elements;

    // Exapand bounds to contain all elements
    for (size_t i = 0; i < elements.size(); i++ )
        expand(elements[i]);

    // Exapand a bit for padding
    expand(0.001f);

    leaf = elements.size() == 1;

    if (!leaf)
        split(_splitAxis);
}

void BVH::split(int _axis) {

    float width = getWidth();
    float height = getHeight();
    float depth = getDepth();
    auto comparator = (width > std::max(height, depth) ) ? Triangle::compareX
                    : (height > std::max(width, depth) ) ? Triangle::compareY
                    : Triangle::compareZ;

    if (_axis = 0) 
        comparator = Triangle::compareX;
    else if (_axis = 1)
        comparator = Triangle::compareY;
    else if (_axis = 2)
        comparator = Triangle::compareZ;

    std::sort(elements.begin(), elements.end(), comparator);
    std::size_t const half_size = elements.size() / 2;
    left = std::make_shared<BVH>( std::vector<Triangle>(elements.begin(), elements.begin() + half_size), _axis );
    left->parent = std::make_shared<BVH>( *this );
    right = std::make_shared<BVH>( std::vector<Triangle>(elements.begin() + half_size, elements.end()), _axis );
    right->parent = std::make_shared<BVH>( *this );
}


void BVH::clear() {
    left = nullptr;
    right = nullptr;
    parent = nullptr;
}

int  BVH::getTotal() {
    if (leaf)
        return elements.size();
    else
        return left->getTotal() + right->getTotal();
}

std::shared_ptr<BVH> BVH::hit(const Ray& _ray, float& _minDistance, float& _maxDistance) {
    if ( !intersection(_ray, (BoundingBox)*this, _minDistance, _maxDistance) )
        return nullptr;

    if (!leaf) {
        float left_minT = _minDistance;
        float left_maxT = _maxDistance;
        std::shared_ptr<BVH> left_hit = left->hit(_ray, left_minT, left_maxT);

        float right_minT = _minDistance;
        float right_maxT = _maxDistance;
        std::shared_ptr<BVH> right_hit = right->hit(_ray, right_minT, right_maxT);

        if (left_hit != nullptr && right_hit != nullptr) {
            // Both hit, get the closer
            if (left_maxT < right_maxT)
                return left_hit;
            else
                return right_hit;
        }
        else if (left_hit != nullptr) 
            return left_hit;
        else if (right_hit != nullptr)
            return right_hit;

        return nullptr;
    }
    else
        return std::make_shared<BVH>( *this );
}

std::shared_ptr<BVH> BVH::closest(const glm::vec3& _point) {
    if ( !contains(_point) )
        return nullptr;

     if (!leaf) {
        std::shared_ptr<BVH> left_c = left->closest(_point);
        std::shared_ptr<BVH> right_c = right->closest(_point);

        if (left_c != nullptr && right_c != nullptr) {
            // // Both hit, get the closer
            // float left_distance = glm::distance(_point, left_c->getCenter());
            // float right_distance = glm::distance(_point, right_c->getCenter());
            // if (left_distance < right_distance)
            //     return left_c;
            // else
            //     return right_c;

            // both hits return it self
            return std::make_shared<BVH>( *this );
        }
        else if (left_c != nullptr) 
            return left_c;
        else if (right_c != nullptr)
            return right_c;

        return nullptr;
    }
    else
        return std::make_shared<BVH>( *this );
}


}