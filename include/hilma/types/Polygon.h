#pragma once

#include "hilma/types/Polyline.h"

namespace hilma {

class Polygon {
public:

    Polygon();
    Polygon( const std::vector< Polyline >& _polygon );
    Polygon( const std::vector< std::vector<glm::vec2> >& _polygon );
    Polygon( const std::vector< std::vector<glm::vec3> >& _polygon );

    void append( const Polyline& _poly ) {
        data.push_back(_poly);
    }

    const Polyline& operator[] (int _index) const {
        return data[_index];
    }

    Polyline& operator[] (int _index) {
        return data[_index];
    }

    size_t size() const {
        return data.size();
    }

private:
    std::vector<Polyline>   data;

};

}