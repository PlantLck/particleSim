#include <SDL2/SDL.h>

struct SimulationConfig;

class InputHandler {
private:
    bool running;
    bool mouseLeftPressed;
    bool mouseRightPressed;
    int mouseX;
    int mouseY;
    int currentMode;
    
public:
    InputHandler() : running(true), mouseLeftPressed(false), 
                     mouseRightPressed(false), mouseX(0), mouseY(0),
                     currentMode(1) {}
    
    bool isRunning() const { return running; }
    bool isMouseLeftPressed() const { return mouseLeftPressed; }
    bool isMouseRightPressed() const { return mouseRightPressed; }
    int getMouseX() const { return mouseX; }
    int getMouseY() const { return mouseY; }
    int getCurrentMode() const { return currentMode; }
    
    void processEvents(SimulationConfig& config) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                handleKeyPress(event.key.keysym.sym, config);
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouseLeftPressed = true;
                }
                else if (event.button.button == SDL_BUTTON_RIGHT) {
                    mouseRightPressed = true;
                }
            }
            else if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouseLeftPressed = false;
                }
                else if (event.button.button == SDL_BUTTON_RIGHT) {
                    mouseRightPressed = false;
                }
            }
            else if (event.type == SDL_MOUSEMOTION) {
                mouseX = event.motion.x;
                mouseY = event.motion.y;
            }
        }
    }
    
private:
    void handleKeyPress(SDL_Keycode key, SimulationConfig& config) {
        switch(key) {
            case SDLK_1:
                currentMode = 1;
                break;
            case SDLK_2:
                currentMode = 2;
                break;
            case SDLK_3:
                currentMode = 3;
                break;
            case SDLK_4:
                currentMode = 4;
                break;
            case SDLK_5:
                currentMode = 5;
                break;
            case SDLK_EQUALS:
                config.increaseParticles(100);
                break;
            case SDLK_MINUS:
                config.decreaseParticles(100);
                break;
            case SDLK_f:
                config.adjustFriction(0.01f);
                break;
            case SDLK_v:
                config.adjustFriction(-0.01f);
                break;
            case SDLK_r:
                config.adjustRestitution(0.05f);
                break;
            case SDLK_e:
                config.adjustRestitution(-0.05f);
                break;
            case SDLK_g:
                config.adjustGravity(1000.0f);
                break;
            case SDLK_b:
                config.adjustGravity(-1000.0f);
                break;
            case SDLK_ESCAPE:
                running = false;
                break;
        }
    }
};