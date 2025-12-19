#include <cstdlib>

struct Particle;
struct SimulationConfig;
struct FrameMetrics;
class SequentialPhysics;
class Renderer;
class UIOverlay;
class InputHandler;
class Timer;
class CSVLogger;

class Simulation {
private:
    int maxParticles;
    int currentCount;
    SimulationConfig* config;
    Particle* particles;
    SequentialPhysics* sequentialPhysics;
    Renderer* renderer;
    UIOverlay* overlay;
    InputHandler* input;
    Timer* physicsTimer;
    Timer* renderTimer;
    CSVLogger* logger;
    FrameMetrics metrics;
    int frameCount;
    
public:
    Simulation(SimulationConfig* cfg, int maxPart) 
        : maxParticles(maxPart), currentCount(cfg->particleCount), config(cfg),
          frameCount(0) {
        
        particles = new Particle[maxParticles];
        initializeParticles(particles, currentCount, cfg->windowWidth, cfg->windowHeight);
        
        sequentialPhysics = new SequentialPhysics(maxParticles);
        renderer = new Renderer(cfg->windowWidth, cfg->windowHeight);
        overlay = new UIOverlay(renderer->getSDLRenderer());
        input = new InputHandler();
        physicsTimer = new Timer();
        renderTimer = new Timer();
        logger = new CSVLogger("data/performance_metrics.csv");
        
        logger->writeHeader();
    }
    
    ~Simulation() {
        delete[] particles;
        delete sequentialPhysics;
        delete renderer;
        delete overlay;
        delete input;
        delete physicsTimer;
        delete renderTimer;
        delete logger;
    }
    
    void run() {
        while (input->isRunning()) {
            input->processEvents(*config);
            
            if (currentCount != config->particleCount) {
                currentCount = config->particleCount;
                initializeParticles(particles, currentCount, 
                                  config->windowWidth, config->windowHeight);
            }
            
            Timer frameTimer;
            frameTimer.start();
            
            physicsTimer->start();
            sequentialPhysics->update(particles, currentCount, *config,
                                     input->isMouseLeftPressed(),
                                     input->isMouseRightPressed(),
                                     input->getMouseX(),
                                     input->getMouseY());
            metrics.physicsTime = physicsTimer->elapsed();
            
            renderTimer->start();
            renderer->clear();
            renderer->drawParticles(particles, currentCount);
            overlay->render(metrics, *config);
            renderer->present();
            metrics.renderTime = renderTimer->elapsed();
            
            metrics.totalTime = frameTimer.elapsed();
            metrics.particleCount = currentCount;
            metrics.currentMode = input->getCurrentMode();
            
            if (frameCount % 60 == 0) {
                logger->logFrame(metrics);
            }
            
            frameCount++;
            
            SDL_Delay(1);
        }
    }
};
