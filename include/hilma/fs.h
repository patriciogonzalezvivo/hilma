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

inline std::string getFilename(const std::string& _filepath) {
    size_t sep = _filepath.find_last_of("\\/");
    std::string out;
    if (sep != std::string::npos)
        out = _filepath.substr(sep + 1, _filepath.size() - sep - 1);

    size_t dot = out.find_last_of(".");
    if (dot != std::string::npos)
        return out.substr(0, dot);
    else
        return out;
}

}
