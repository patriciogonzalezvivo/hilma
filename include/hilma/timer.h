#pragma once

#include <chrono>

namespace hilma
{

class Timer {
public:
    void start() { t0 = std::chrono::high_resolution_clock::now(); }
    void stop() { timestamp = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count() * 1000.0; }
    const double & get() { return timestamp; }

private:
    std::chrono::high_resolution_clock::time_point t0;
    double timestamp = 0.0;
};
    
}