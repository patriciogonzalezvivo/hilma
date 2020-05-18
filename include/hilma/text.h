#pragma once

#include "glm/glm.hpp"

#include <algorithm> 
#include <iostream>
#include <sstream>

#include <iomanip>
#include <cctype>
#include <vector>

namespace hilma {

const std::string DELETE_LINE = "\e[2K\r\e[1A";

//-------------------------------------------------- << and >>

inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
    os << vec.x << ", " << vec.y << ", " << vec.z;
    return os;
}

inline std::istream& operator>>(std::istream& is, glm::vec3& vec) {
    is >> vec.x;
    is.ignore(2);
    is >> vec.y;
    is.ignore(2);
    is >> vec.z;
    return is;
}

//----------------------------------------  String operations

inline std::string toLower(const std::string& _string) {
    std::string std = _string;
    for (int i = 0; _string[i]; i++) {
        std[i] = tolower(_string[i]);
    }
    return std;
}

inline std::string toUpper(const std::string& _string) {
    std::string std = _string;
    for (int i = 0; _string[i]; i++) {
        std[i] = toupper(_string[i]);
    }
    return std;
}

inline std::string toUnderscore(const std::string& _string){
    std::string std = _string;
    std::replace(std.begin(), std.end(), ' ', '_');
    return std;
}

inline std::string purifyString(const std::string& _string) {
    std::string std = _string;
    for (std::string::iterator it = std.begin(), itEnd = std.end(); it!=itEnd; ++it) {
        if (static_cast<uint32_t>(*it) < 32 || 
            static_cast<uint32_t>(*it) > 127 || 
            *it == '.' ||
            *it == '-' ||
            *it == '\\'||
            *it == '/' ) {
            (*it) = '_';
        }
    }
    return std;
}

inline std::vector<std::string> split(const std::string& _string, char _sep, bool _tolerate_empty) {
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = _string.find(_sep, start)) != std::string::npos) {
        if (end != start || _tolerate_empty) {
          tokens.push_back(_string.substr(start, end - start));
        }
        start = end + 1;
    }
    if (end != start || _tolerate_empty) {
       tokens.push_back(_string.substr(start));
    }
    return tokens;
}

//---------------------------------------- Conversions

inline int toInt(const std::string &_intString) {
    int x = 0;
    std::istringstream cur(_intString);
    cur >> x;
    return x;
}

inline float toFloat(const std::string &_floatString) {
    float x = 0;
    std::istringstream cur(_floatString);
    cur >> x;
    return x;
}

inline double toDouble(const std::string &_doubleString) {
    double x = 0;
    std::istringstream cur(_doubleString);
    cur >> x;
    return x;
}

inline bool toBool(const std::string &_boolString) {
    static const std::string trueString = "true";
    static const std::string falseString = "false";
    
    std::string lower = toLower(_boolString);
    
    if(lower == trueString) {
        return true;
    }
    if(lower == falseString) {
        return false;
    }
    
    bool x = false;
    std::istringstream cur(lower);
    cur >> x;
    return x;
}

inline char ofToChar(const std::string &_charString) {
    char x = '\0';
    std::istringstream cur(_charString);
    cur >> x;
    return x;
}

inline std::string toString(bool _bool){
    std::ostringstream strStream;
    strStream << (_bool?"true":"false") ;
    return strStream.str();
}

template <class T>
std::string toString(const T& _value){
    std::ostringstream out;
    out << _value;
    return out.str();
}

/// like sprintf "%4f" format, in this example precision=4
template <class T>
std::string toString(const T& _value, int _precision){
    std::ostringstream out;
    out << std::fixed << std::setprecision(_precision) << _value;
    return out.str();
}

/// like sprintf "% 4d" or "% 4f" format, in this example width=4, fill=' '
template <class T>
std::string toString(const T& _value, int _width, char _fill ){
    std::ostringstream out;
    out << std::fixed << std::setfill(_fill) << std::setw(_width) << _value;
    return out.str();
}

/// like sprintf "%04.2d" or "%04.2f" format, in this example precision=2, width=4, fill='0'
template <class T>
std::string toString(const T& _value, int _precision, int _width, char _fill ){
    std::ostringstream out;
    out << std::fixed << std::setfill(_fill) << std::setw(_width) << std::setprecision(_precision) << _value;
    return out.str();
}

inline std::string toString(const glm::vec2 &_vec, char _sep = ','){
    std::ostringstream strStream;
    strStream<< _vec.x << _sep << _vec.y << _sep;
    return strStream.str();
}

inline std::string toString(const glm::vec3 &_vec, char _sep = ','){
    std::ostringstream strStream;
    strStream<< _vec.x << _sep << _vec.y << _sep << _vec.z;
    return strStream.str();
}

inline std::string toString(const glm::vec4 &_vec, char _sep = ','){
    std::ostringstream strStream;
    strStream<< _vec.x << _sep << _vec.y << _sep << _vec.z << _sep << _vec.w;
    return strStream.str();
}

inline void printProgressBar(const std::string& _name, float _pct, int total_bars = 50 ) {
    std::cout << DELETE_LINE;
    std::cout << _name << " [ ";
    _pct *= total_bars;
    for (int i = 0; i < total_bars; i++) {
        if (i < _pct) std::cout << "#";
        else std::cout << ".";
    }
    std::cout << " ]" << std::endl;
}

}