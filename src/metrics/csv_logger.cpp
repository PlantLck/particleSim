#include <fstream>
#include <string>
#include <iomanip>

class CSVLogger {
private:
    std::ofstream file;
    bool headerWritten;
    
public:
    CSVLogger(const std::string& filename) : headerWritten(false) {
        file.open(filename, std::ios::out | std::ios::app);
    }
    
    ~CSVLogger() {
        if (file.is_open()) {
            file.close();
        }
    }
    
    void writeHeader() {
        if (!headerWritten && file.is_open()) {
            file << "Timestamp,Mode,ParticleCount,PhysicsTime,RenderTime,TotalTime,FPS\n";
            headerWritten = true;
        }
    }
    
    void logFrame(const FrameMetrics& metrics) {
        if (file.is_open()) {
            auto now = std::chrono::system_clock::now();
            auto timestamp = std::chrono::system_clock::to_time_t(now);
            
            double fps = metrics.totalTime > 0 ? 1000.0 / metrics.totalTime : 0;
            
            file << timestamp << ","
                 << metrics.currentMode << ","
                 << metrics.particleCount << ","
                 << std::fixed << std::setprecision(3)
                 << metrics.physicsTime << ","
                 << metrics.renderTime << ","
                 << metrics.totalTime << ","
                 << std::setprecision(1) << fps << "\n";
        }
    }
    
    void flush() {
        if (file.is_open()) {
            file.flush();
        }
    }
};