#include "particle.cpp"
#include "core/config.cpp"
#include "metrics/timer.cpp"
#include "metrics/csv_logger.cpp"
#include "physics/sequential.cpp"
#include "rendering/renderer.cpp"
#include "rendering/ui_overlay.cpp"
#include "core/input_handler.cpp"
#include "core/simulation.cpp"

int main(int argc, char* argv[]) {
    SimulationConfig config;
    
    const int MAX_PARTICLES = 10000;
    
    Simulation simulation(&config, MAX_PARTICLES);
    simulation.run();
    
    return 0;
}