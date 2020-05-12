#pragma once

#include <string>

inline std::string getBaseDir(const std::string& _filepath) {
    std::string base_dir = "";

    if (_filepath.find_last_of("/\\") != std::string::npos)
        base_dir = _filepath.substr(0, _filepath.find_last_of("/\\"));
    else 
        base_dir = ".";
    
#ifdef PLATFORM_WIN32
    base_dir += "\\";
#else
    base_dir += "/";
#endif

    return base_dir;
}