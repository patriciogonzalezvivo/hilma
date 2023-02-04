#pragma once

#include "hilma/types/polyline.h"

namespace hilma {

class Polygon {
public:

    Polygon();
    Polygon( const std::vector< Polyline >& _polygon );
    Polygon( const std::vector< std::vector<glm::vec2> >& _polygon );
    Polygon( const std::vector< std::vector<glm::vec3> >& _polygon );

    void append( const Polyline& _poly );
    void append( const Polygon& _poly );
    void append( const std::vector< std::vector<glm::vec2> >& _polygon );
    void append( const std::vector< std::vector<glm::vec3> >& _polygon );

    const Polyline& operator[] (int _index) const { return data[_index]; }
    Polyline&   operator[] (int _index) { return data[_index]; }

    const Polyline& get(int _index) const { return data[_index]; }
    Polyline&   get(int _index) { return data[_index]; }

    size_t size() const { return data.size(); }

private:
    std::vector<Polyline>   data;

};

}