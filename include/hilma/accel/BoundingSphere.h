#pragma once

#include "glm/glm.hpp"

namespace hilma {

class BoundingSphere {
public:

	BoundingSphere() : center(0.0f), radio(0.0) {}
	BoundingSphere(glm::vec3 const& x, float r) : center(x), radio(r) {}

	glm::vec3 const& getCenter() const { return center; }
	glm::vec3& getCenter() { return center; }

	float getRadio() const { return radio; }
	float& getRadio() { return radio; }

	bool overlaps(BoundingSphere const& _other) const { 
		float rr = radio + _other.radio;
        glm::vec3 c = center - _other.center;
		return glm::dot(c,c) < rr * rr; 
	}

	bool contains(BoundingSphere const& _other) const {
		float rr = getRadio() - _other.getRadio();
        glm::vec3 c = getCenter() - _other.getCenter();
		return glm::dot(c, c) < rr * rr;
	}

	bool contains(glm::vec3 const& _other) const {
        glm::vec3 c = getCenter() - _other;
		return glm::dot(c,c) < radio * radio;
	}

private:

	glm::vec3   center;
	float       radio;

};

}