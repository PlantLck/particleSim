struct SimulationConfig {
    int particleCount;
    float friction;
    float restitution;
    float gravityStrength;
    float deltaTime;
    int windowWidth;
    int windowHeight;
    
    SimulationConfig() 
        : particleCount(1000),
          friction(0.99f),
          restitution(0.8f),
          gravityStrength(5000.0f),
          deltaTime(0.016f),
          windowWidth(1280),
          windowHeight(720) {}
    
    void increaseParticles(int amount) {
        particleCount += amount;
        if (particleCount > 10000) particleCount = 10000;
    }
    
    void decreaseParticles(int amount) {
        particleCount -= amount;
        if (particleCount < 100) particleCount = 100;
    }
    
    void adjustFriction(float delta) {
        friction += delta;
        if (friction < 0.8f) friction = 0.8f;
        if (friction > 1.0f) friction = 1.0f;
    }
    
    void adjustRestitution(float delta) {
        restitution += delta;
        if (restitution < 0.1f) restitution = 0.1f;
        if (restitution > 1.0f) restitution = 1.0f;
    }
    
    void adjustGravity(float delta) {
        gravityStrength += delta;
        if (gravityStrength < 1000.0f) gravityStrength = 1000.0f;
        if (gravityStrength > 20000.0f) gravityStrength = 20000.0f;
    }
};