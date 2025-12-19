#include <SDL2/SDL.h>
#include <string>
#include <sstream>
#include <iomanip>

struct FrameMetrics;
struct SimulationConfig;

class UIOverlay {
private:
    SDL_Renderer* renderer;
    
    void drawText(const std::string& text, int x, int y) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect rect = {x, y, static_cast<int>(text.length() * 8), 16};
        SDL_RenderDrawRect(renderer, &rect);
    }
    
public:
    UIOverlay(SDL_Renderer* r) : renderer(r) {}
    
    void render(const FrameMetrics& metrics, const SimulationConfig& config) {
        std::ostringstream oss;
        
        double fps = metrics.totalTime > 0 ? 1000.0 / metrics.totalTime : 0;
        
        oss << "Mode: " << getModeString(metrics.currentMode);
        drawText(oss.str(), 10, 10);
        oss.str("");
        
        oss << "Particles: " << metrics.particleCount;
        drawText(oss.str(), 10, 30);
        oss.str("");
        
        oss << std::fixed << std::setprecision(2);
        oss << "Physics: " << metrics.physicsTime << " ms";
        drawText(oss.str(), 10, 50);
        oss.str("");
        
        oss << "Render: " << metrics.renderTime << " ms";
        drawText(oss.str(), 10, 70);
        oss.str("");
        
        oss << std::setprecision(1);
        oss << "FPS: " << fps;
        drawText(oss.str(), 10, 90);
        oss.str("");
        
        oss << std::setprecision(2);
        oss << "Friction: " << config.friction;
        drawText(oss.str(), 10, 120);
        oss.str("");
        
        oss << "Restitution: " << config.restitution;
        drawText(oss.str(), 10, 140);
        oss.str("");
        
        oss << std::setprecision(0);
        oss << "Gravity: " << config.gravityStrength;
        drawText(oss.str(), 10, 160);
    }
    
private:
    std::string getModeString(int mode) {
        switch(mode) {
            case 1: return "Sequential";
            case 2: return "OpenMP";
            case 3: return "MPI";
            case 4: return "CUDA Basic";
            case 5: return "CUDA Optimized";
            default: return "Unknown";
        }
    }
};