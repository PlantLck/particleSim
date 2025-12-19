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
    int windowWidth;
    int windowHeight;
    
    void drawFilledRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        SDL_Rect rect = {x, y, w, h};
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
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
        
        int height = surface->h;
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        
        return height;
    }
    
    void drawHorizontalLine(int x1, int x2, int y, Uint8 r, Uint8 g, Uint8 b) {
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, x1, y, x2, y);
    }
    
public:
    UIOverlay(Renderer* r) {
        renderer = r->getSDLRenderer();
        font = r->getFont();
        titleFont = r->getTitleFont();
        windowWidth = r->getWidth();
        windowHeight = r->getHeight();
    }
    
    void render(const FrameMetrics& metrics, const SimulationConfig& config) {
        renderLeftPanel(metrics);
        renderRightPanel(metrics, config);
    }
    
private:
    void renderLeftPanel(const FrameMetrics& metrics) {
        const int PANEL_X = 10;
        const int PANEL_Y = 10;
        const int PANEL_W = 200;
        const int PANEL_H = 180;
        
        drawFilledRect(PANEL_X, PANEL_Y, PANEL_W, PANEL_H, 20, 20, 25, 200);
        
        int yPos = PANEL_Y + 10;
        const int LINE_HEIGHT = 18;
        const int INDENT = PANEL_X + 10;
        
        drawText("PERFORMANCE", INDENT, yPos, 150, 200, 255, titleFont);
        yPos += LINE_HEIGHT + 5;
        
        drawHorizontalLine(INDENT, INDENT + PANEL_W - 20, yPos, 60, 80, 120);
        yPos += 8;
        
        std::ostringstream oss;
        
        oss << "Mode: " << getModeString(metrics.currentMode);
        yPos += drawText(oss.str(), INDENT, yPos, 200, 200, 200);
        yPos += 3;
        oss.str("");
        
        oss << "Particles: " << metrics.particleCount;
        yPos += drawText(oss.str(), INDENT, yPos, 200, 200, 200);
        yPos += 8;
        oss.str("");
        
        oss << std::fixed << std::setprecision(2);
        oss << "Physics: " << metrics.physicsTime << " ms";
        yPos += drawText(oss.str(), INDENT, yPos, 180, 220, 180);
        yPos += 3;
        oss.str("");
        
        oss << "Render: " << metrics.renderTime << " ms";
        yPos += drawText(oss.str(), INDENT, yPos, 180, 220, 180);
        yPos += 3;
        oss.str("");
        
        oss << "Total: " << metrics.totalTime << " ms";
        yPos += drawText(oss.str(), INDENT, yPos, 180, 220, 180);
        yPos += 8;
        oss.str("");
        
        double fps = metrics.totalTime > 0 ? 1000.0 / metrics.totalTime : 0;
        oss << std::setprecision(1);
        oss << "FPS: " << fps;
        drawText(oss.str(), INDENT, yPos, 100, 255, 150);
    }
    
    void renderRightPanel(const FrameMetrics& metrics, const SimulationConfig& config) {
        const int PANEL_W = 220;
        const int PANEL_X = windowWidth - PANEL_W - 10;
        const int PANEL_Y = 10;
        const int PANEL_H = 340;
        
        drawFilledRect(PANEL_X, PANEL_Y, PANEL_W, PANEL_H, 20, 20, 25, 200);
        
        int yPos = PANEL_Y + 10;
        const int LINE_HEIGHT = 18;
        const int INDENT = PANEL_X + 10;
        
        drawText("CONTROLS", INDENT, yPos, 150, 200, 255, titleFont);
        yPos += LINE_HEIGHT + 5;
        
        drawHorizontalLine(INDENT, INDENT + PANEL_W - 20, yPos, 60, 80, 120);
        yPos += 8;
        
        yPos += drawText("[M] Toggle Menu", INDENT, yPos, 180, 180, 180);
        yPos += 3;
        yPos += drawText("[SPACE] Pause/Resume", INDENT, yPos, 180, 180, 180);
        yPos += 3;
        yPos += drawText("[R] Reset Simulation", INDENT, yPos, 180, 180, 180);
        yPos += 3;
        yPos += drawText("[+] Add/Remove Particles", INDENT, yPos, 180, 180, 180);
        yPos += 8;
        yPos += drawText("[1-5] Change Parallel Mode", INDENT, yPos, 180, 180, 180);
        yPos += 3;
        yPos += drawText("[F/G] Increase/Decrease", INDENT, yPos, 180, 180, 180);
        yPos += 3;
        yPos += drawText("      Friction", INDENT, yPos, 180, 180, 180);
        yPos += 3;
        yPos += drawText("[S/D] Adjust Particles", INDENT, yPos, 180, 180, 180);
        yPos += 10;
        
        drawText("CURRENT SETTINGS", INDENT, yPos, 150, 200, 255, titleFont);
        yPos += LINE_HEIGHT + 5;
        
        drawHorizontalLine(INDENT, INDENT + PANEL_W - 20, yPos, 60, 80, 120);
        yPos += 8;
        
        std::ostringstream oss;
        oss << "Particles: " << metrics.particleCount;
        yPos += drawText(oss.str(), INDENT, yPos, 200, 200, 200);
        yPos += 3;
        oss.str("");
        
        oss << std::fixed << std::setprecision(4);
        oss << "Friction: " << config.friction;
        yPos += drawText(oss.str(), INDENT, yPos, 200, 200, 200);
        yPos += 3;
        oss.str("");
        
        oss << std::setprecision(3);
        oss << "Restitution: " << config.restitution;
        yPos += drawText(oss.str(), INDENT, yPos, 200, 200, 200);
        yPos += 3;
        oss.str("");
        
        oss << std::setprecision(0);
        oss << "Gravity: " << config.gravityStrength;
        drawText(oss.str(), INDENT, yPos, 200, 200, 200);
    }
    
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
