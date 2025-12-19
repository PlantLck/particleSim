#include <SDL2/SDL.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

struct FrameMetrics;
struct SimulationConfig;

class UIOverlay {
private:
    SDL_Renderer* renderer;
    
    void drawFilledRect(int x, int y, int w, int h, SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_Rect rect = {x, y, w, h};
        SDL_RenderFillRect(renderer, &rect);
    }
    
    void drawText(const std::string& text, int x, int y, SDL_Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        
        // Simple pixel-based text rendering using rectangles
        const int charWidth = 8;
        const int charHeight = 12;
        
        for (size_t i = 0; i < text.length(); i++) {
            SDL_Rect charRect = {
                x + static_cast<int>(i) * charWidth,
                y,
                charWidth - 2,
                charHeight
            };
            SDL_RenderDrawRect(renderer, &charRect);
        }
    }
    
    void drawInfoPanel(int x, int y, int width, int height) {
        // Draw semi-transparent background panel
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_Color bgColor = {28, 28, 36, 220};
        drawFilledRect(x, y, width, height, bgColor);
        
        // Draw border
        SDL_SetRenderDrawColor(renderer, 80, 120, 200, 255);
        SDL_Rect borderRect = {x, y, width, height};
        SDL_RenderDrawRect(renderer, &borderRect);
        
        // Draw accent line at top
        SDL_SetRenderDrawColor(renderer, 100, 150, 255, 255);
        SDL_RenderDrawLine(renderer, x, y, x + width, y);
        
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
    
public:
    UIOverlay(SDL_Renderer* r) : renderer(r) {}
    
    void render(const FrameMetrics& metrics, const SimulationConfig& config) {
        const int PANEL_X = 10;
        const int PANEL_Y = 10;
        const int PANEL_WIDTH = 280;
        const int PANEL_HEIGHT = 240;
        const int TEXT_X = PANEL_X + 15;
        const int LINE_HEIGHT = 20;
        
        // Draw background panel
        drawInfoPanel(PANEL_X, PANEL_Y, PANEL_WIDTH, PANEL_HEIGHT);
        
        // Prepare text lines
        std::vector<std::string> lines;
        std::ostringstream oss;
        
        // Title
        lines.push_back("PERFORMANCE METRICS");
        lines.push_back("");
        
        // Mode
        oss << "Mode: " << getModeString(metrics.currentMode);
        lines.push_back(oss.str());
        oss.str("");
        
        // Particle count
        oss << "Particles: " << metrics.particleCount;
        lines.push_back(oss.str());
        oss.str("");
        
        lines.push_back("");
        
        // Physics time
        oss << std::fixed << std::setprecision(2);
        oss << "Physics:  " << metrics.physicsTime << " ms";
        lines.push_back(oss.str());
        oss.str("");
        
        // Render time
        oss << "Render:   " << metrics.renderTime << " ms";
        lines.push_back(oss.str());
        oss.str("");
        
        // Total time
        oss << "Total:    " << metrics.totalTime << " ms";
        lines.push_back(oss.str());
        oss.str("");
        
        lines.push_back("");
        
        // FPS
        double fps = metrics.totalTime > 0 ? 1000.0 / metrics.totalTime : 0;
        oss << std::setprecision(1);
        oss << "FPS: " << fps;
        lines.push_back(oss.str());
        oss.str("");
        
        lines.push_back("");
        
        // Configuration parameters
        oss << std::setprecision(3);
        oss << "Friction:     " << config.friction;
        lines.push_back(oss.str());
        oss.str("");
        
        oss << "Restitution:  " << config.restitution;
        lines.push_back(oss.str());
        oss.str("");
        
        oss << std::setprecision(0);
        oss << "Gravity:      " << config.gravityStrength;
        lines.push_back(oss.str());
        
        // Render text lines
        SDL_Color textColor = {220, 220, 230, 255};
        SDL_Color titleColor = {150, 200, 255, 255};
        
        int currentY = PANEL_Y + 15;
        for (size_t i = 0; i < lines.size(); i++) {
            if (i == 0) {
                drawText(lines[i], TEXT_X, currentY, titleColor);
            } else if (!lines[i].empty()) {
                drawText(lines[i], TEXT_X, currentY, textColor);
            }
            currentY += LINE_HEIGHT;
        }
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
