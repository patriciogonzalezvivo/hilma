#pragma once

#include <memory>
#include "glm/glm.hpp"

namespace hilma {

class Image {
public:

    Image();
    Image(size_t _width, size_t _height, size_t _channels);
    virtual ~Image();

    bool        allocate(size_t _width, size_t _height, size_t _channels);
    bool        isAllocated() const { return data != nullptr; }

    void        deAllocate();

    int         getWidth() const { return width;}
    int         getHeight() const { return height;};
    int         getChannels() const { return channels;};
    size_t      getIndex(size_t _x, size_t _y) const { return (_y * width + _x) * channels; };
    size_t      getIndexUV(float _u, float _v) const { return getIndex(_u * width, _v * height); }

    float       getData(size_t _index) const;
    glm::vec4   getColor(size_t index) const;
    
    float       getData(size_t _x, size_t _y) const { return getData(getIndex(_x, _y)); };
    glm::vec4   getColor(size_t _x, size_t _y) const { return getColor(getIndex(_x, _y)); };

    void        setData(size_t _index, float _data);
    void        setData(size_t _index, const float* _array1D, int _n);
    void        setData(size_t _x, size_t _y, const float* _array1D, int _n ) { setData(getIndex(_x, _y), _array1D, _n); }
    void        setColor(size_t _x, size_t _y, const glm::vec3& _color) { setData(_x, _y, &_color[0], 3); }
    void        setColor(size_t _x, size_t _y, const glm::vec4& _color) { setData(_x, _y, &_color[0], std::min(4, channels)); }

private:
    float*      data;
    int         width;
    int         height;
    int         channels;

    friend bool saveJpg( const std::string&, Image& );
    friend bool loadJpg( const std::string&, Image&, int );
    friend bool savePng( const std::string&, Image& );
    friend bool loadPng( const std::string&, Image&, int );
    friend bool loadHdr( const std::string&, Image& );
    friend bool saveHdr( const std::string&, Image& );
    
};

typedef std::shared_ptr<Image> ImagePtr;
typedef std::shared_ptr<Image const> ImageConstPtr;

}