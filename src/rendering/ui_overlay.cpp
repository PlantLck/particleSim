#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

struct FrameMetrics;
struct SimulationConfig;
class Renderer;

class UIOverlay {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_Font* titleFont;
    
    void drawFilledRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_Rect rect = {x, y, w, h};
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
    
    void drawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_Rect rect = {x, y, w, h};
        SDL_RenderDrawRect(renderer, &rect);
    }
    
    int drawText(const std::string& text, int x, int y, 
                 Uint8 r, Uint8 g, Uint8 b, TTF_Font* useFont = nullptr) {
        if (!useFont) useFont = font;
        if (!useFont) return 0;
        
        SDL_Color color = {r, g, b, 255};
        SDL_Surface* surface = TTF_RenderText_Blended(useFont, text.c_str(), color);
        if (!surface) return 0;
        
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            SDL_FreeSurface(surface);
            return 0;
        }
        
        SDL_Rect destRect = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
        
        int width = surface->w;
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        
        return width;
    }
    
    void drawPanel(int x, int y, int w, int h, const std::string& title) {
        // Background
        drawFilledRect(x, y, w, h, 25, 25, 35, 230);
        
        // Border
        drawRect(x, y, w, h, 60, 100, 180, 255);
        
        // Title bar
        drawFilledRect(x, y, w, 30, 35, 35, 50, 255);
        drawRect(x, y, w, 30, 70, 110, 200, 255);
        
        // Title text
        drawText(title, x + 10, y + 7, 150, 200, 255, titleFont);
    }
    
public:
    UIOverlay(Renderer* r) {
        renderer = r->getSDLRenderer();
        font = r->getFont();
        titleFont = r->getTitleFont();
    }
    
    void render(const FrameMetrics& metrics, const SimulationConfig& config) {
        const int PANEL_X = 15;
        const int PANEL_Y = 15;
        const int PANEL_W = 300;
        const int PANEL_H = 360;
        
        // Draw main stats panel
        drawPanel(PANEL_X, PANEL_Y, PANEL_W, PANEL_H, "PERFORMANCE METRICS");
        
        int yOffset = PANEL_Y + 45;
        const int LINE_HEIGHT = 22;
        const int INDENT = PANEL_X + 15;
        
        // Mode information
        std::ostringstream oss;
        oss << "Mode: " << getModeString(metrics.currentMode);
        drawText(oss.str(), INDENT, yOffset, 200, 220, 240);
        yOffset += LINE_HEIGHT;
        
        // Particle count
        oss.str("");
        oss << "Particles: " << metrics.particleCount;
        drawText(oss.str(), INDENT, yOffset, 200, 220, 240);
        yOffset += LINE_HEIGHT + 10;
        
        // Section header
        drawText("Timing (ms)", INDENT, yOffset, 120, 180, 255);
        yOffset += LINE_HEIGHT;
        
        // Physics time
        oss.str("");
        oss << std::fixed << std::setprecision(3);
        oss << "  Physics:  " << std::setw(7) << metrics.physicsTime;
        drawText(oss.str(), INDENT, yOffset, 180, 200, 220);
        yOffset += LINE_HEIGHT;
        
        // Render time
        oss.str("");
        oss << "  Render:   " << std::setw(7) << metrics.renderTime;
        drawText(oss.str(), INDENT, yOffset, 180, 200, 220);
        yOffset += LINE_HEIGHT;
        
        // Total time
        oss.str("");
        oss << "  Total:    " << std::setw(7) << metrics.totalTime;
        drawText(oss.str(), INDENT, yOffset, 180, 200, 220);
        yOffset += LINE_HEIGHT + 10;
        
        // FPS
        double fps = metrics.totalTime > 0 ? 1000.0 / metrics.totalTime : 0;
        oss.str("");
        oss << std::setprecision(1);
        oss << "FPS: " << fps;
        drawText(oss.str(), INDENT, yOffset, 100, 255, 150);
        yOffset += LINE_HEIGHT + 15;
        
        // Configuration section
        drawText("Configuration", INDENT, yOffset, 120, 180, 255);
        yOffset += LINE_HEIGHT;
        
        oss.str("");
        oss << std::fixed << std::setprecision(4);
        oss << "  Friction:     " << config.friction;
        drawText(oss.str(), INDENT, yOffset, 180, 200, 220);
        yOffset += LINE_HEIGHT;
        
        oss.str("");
        oss << std::setprecision(3);
        oss << "  Restitution:  " << config.restitution;
        drawText(oss.str(), INDENT, yOffset, 180, 200, 220);
        yOffset += LINE_HEIGHT;
        
        oss.str("");
        oss << std::setprecision(0);
        oss << "  Gravity:      " << config.gravityStrength;
        drawText(oss.str(), INDENT, yOffset, 180, 200, 220);
        yOffset += LINE_HEIGHT + 15;
        
        // Controls hint
        drawText("Press keys 1-5 to switch modes", INDENT, yOffset, 140, 160, 180);
        yOffset += LINE_HEIGHT;
        drawText("Use +/- to adjust particles", INDENT, yOffset, 140, 160, 180);
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
