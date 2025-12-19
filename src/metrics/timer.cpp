#include <chrono>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point startTime;
    
public:
    void start() {
        startTime = std::chrono::high_resolution_clock::now();
    }
    
    double elapsed() {
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = endTime - startTime;
        return duration.count();
    }
};

struct FrameMetrics {
    double physicsTime;
    double renderTime;
    double totalTime;
    int particleCount;
    int currentMode;
    
    FrameMetrics() : physicsTime(0), renderTime(0), totalTime(0), particleCount(0), currentMode(1) {}
};