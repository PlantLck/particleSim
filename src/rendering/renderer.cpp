#include <SDL2/SDL.h>
#include <cmath>
#include <algorithm>

struct Vec2;
struct Particle;

class Renderer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width;
    int height;
    
    void drawFilledCircle(int centerX, int centerY, int radius) {
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
    Renderer(int w, int h) : width(w), height(h) {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Parallel Particle Simulation - Edge Devices",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   width, height,
                                   SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, 
                                      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    }
    
    ~Renderer() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    
    void clear() {
        SDL_SetRenderDrawColor(renderer, 18, 18, 24, 255);
        SDL_RenderClear(renderer);
    }
    
    void drawParticles(const Particle* particles, int count) {
        const int PARTICLE_RADIUS = 4;
        
        for (int i = 0; i < count; i++) {
            float speed = particles[i].velocity.length();
            
            // Color particles based on velocity magnitude for visual feedback
            int colorIntensity = std::min(255, static_cast<int>(speed * 3));
            
            // Blue to cyan to white gradient based on speed
            int r = 80 + colorIntensity / 3;
            int g = 120 + colorIntensity / 2;
            int b = 200 + colorIntensity / 5;
            
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            drawFilledCircle(static_cast<int>(particles[i].position.x),
                           static_cast<int>(particles[i].position.y),
                           PARTICLE_RADIUS);
        }
    }
    
    void present() {
        SDL_RenderPresent(renderer);
    }
    
    SDL_Renderer* getSDLRenderer() {
        return renderer;
    }
};
