#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <algorithm>
#include <string>
#include <random>

struct Vec2;
struct Particle;

class Renderer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width;
    int height;
    TTF_Font* font;
    TTF_Font* titleFont;
    std::mt19937 colorGen;
    
    struct Color {
        Uint8 r, g, b;
    };
    
    Color getParticleColor(int index) {
        // Varied particle colors: white, cyan, pink, yellow, light blue
        static const Color colors[] = {
            {255, 255, 255},  // White
            {100, 255, 255},  // Cyan
            {255, 100, 255},  // Pink
            {255, 255, 100},  // Yellow
            {150, 200, 255},  // Light blue
            {200, 255, 200},  // Light green
        };
        return colors[index % 6];
    }
    
    void drawFilledCircle(int centerX, int centerY, int radius, Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w;
                int dy = radius - h;
                if ((dx*dx + dy*dy) <= (radius * radius)) {
                    SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
                }
            }
        }
    }
    
public:
    Renderer(int w, int h) : width(w), height(h), font(nullptr), titleFont(nullptr) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::runtime_error("SDL initialization failed");
        }
        
        if (TTF_Init() < 0) {
            throw std::runtime_error("SDL_ttf initialization failed");
        }
        
        window = SDL_CreateWindow("Parallel Particle Simulation - C++",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   width, height,
                                   SDL_WINDOW_SHOWN);
        
        if (!window) {
            throw std::runtime_error("Window creation failed");
        }
        
        renderer = SDL_CreateRenderer(window, -1, 
                                      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        
        if (!renderer) {
            throw std::runtime_error("Renderer creation failed");
        }
        
        const char* fontPaths[] = {
            "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "/usr/share/fonts/TTF/DejaVuSansMono.ttf",
            "/System/Library/Fonts/Monaco.ttf",
            "C:\\Windows\\Fonts\\consola.ttf"
        };
        
        for (const char* path : fontPaths) {
            font = TTF_OpenFont(path, 13);
            if (font) break;
        }
        
        for (const char* path : fontPaths) {
            titleFont = TTF_OpenFont(path, 14);
            if (titleFont) break;
        }
        
        if (!font || !titleFont) {
            throw std::runtime_error("Failed to load fonts");
        }
        
        std::random_device rd;
        colorGen.seed(rd());
    }
    
    ~Renderer() {
        if (font) TTF_CloseFont(font);
        if (titleFont) TTF_CloseFont(titleFont);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }
    
    void clear() {
        SDL_SetRenderDrawColor(renderer, 10, 10, 15, 255);
        SDL_RenderClear(renderer);
    }
    
    void drawParticles(const Particle* particles, int count) {
        const int PARTICLE_RADIUS = 3;
        
        for (int i = 0; i < count; i++) {
            Color color = getParticleColor(i);
            drawFilledCircle(static_cast<int>(particles[i].position.x),
                           static_cast<int>(particles[i].position.y),
                           PARTICLE_RADIUS,
                           color);
        }
    }
    
    void present() {
        SDL_RenderPresent(renderer);
    }
    
    SDL_Renderer* getSDLRenderer() {
        return renderer;
    }
    
    TTF_Font* getFont() {
        return font;
    }
    
    TTF_Font* getTitleFont() {
        return titleFont;
    }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};
