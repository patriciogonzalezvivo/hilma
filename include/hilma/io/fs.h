#pragma once

#include <string>

inline bool haveExt(const std::string& _file, const std::string& _ext){
    return _file.find( "." + _ext) != std::string::npos;
}

inline std::string getExt(const std::string& _filename) {
    if (_filename.find_last_of(".") != std::string::npos)
        return _filename.substr(_filename.find_last_of(".") + 1);
    return "";
}
