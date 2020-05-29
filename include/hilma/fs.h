#pragma once

#include <string>
#include <sys/stat.h>
// #include <fstream>      // File

namespace hilma {

inline bool urlExists(const std::string& _name) {
    struct stat buffer;
    return (stat (_name.c_str(), &buffer) == 0);
}

inline bool haveExt(const std::string& _file, const std::string& _ext){
    return _file.find( "." + _ext) != std::string::npos;
}

inline std::string getExt(const std::string& _filename) {
    if (_filename.find_last_of(".") != std::string::npos)
        return _filename.substr(_filename.find_last_of(".") + 1);
    return "";
}

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

}
