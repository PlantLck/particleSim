#include <cmath>
#include <algorithm>

struct Vec2;
struct Particle;
struct SimulationConfig;

class SequentialPhysics {
private:
    Vec2* forces;
    int maxParticles;
    
public:
    SequentialPhysics(int maxParticles) : maxParticles(maxParticles) {
        forces = new Vec2[maxParticles];
    }
    
    ~SequentialPhysics() {
        delete[] forces;
    }
    
    void update(Particle* particles, int count, const SimulationConfig& config,
                bool mouseLeft, bool mouseRight, int mouseX, int mouseY) {
        
        for (int i = 0; i < count; i++) {
            forces[i] = Vec2(0, 0);
        }
        
        if (mouseLeft || mouseRight) {
            Vec2 mousePos(mouseX, mouseY);
            float sign = mouseLeft ? 1.0f : -1.0f;
            
            for (int i = 0; i < count; i++) {
                Vec2 delta = mousePos - particles[i].position;
                float distSq = delta.lengthSquared();
                
                if (distSq > 1.0f) {
                    float forceMag = sign * config.gravityStrength * particles[i].mass / distSq;
                    Vec2 forceDir = delta.normalized();
                    forces[i] += forceDir * forceMag;
                }
            }
        }
        
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                Vec2 delta = particles[j].position - particles[i].position;
                float distSq = delta.lengthSquared();
                float minDist = 5.0f;
                float minDistSq = minDist * minDist;
                
                if (distSq < minDistSq && distSq > 0.01f) {
                    float dist = std::sqrt(distSq);
                    float overlap = minDist - dist;
                    Vec2 normal = delta.normalized();
                    
                    Vec2 relVel = particles[j].velocity - particles[i].velocity;
                    float velAlongNormal = relVel.x * normal.x + relVel.y * normal.y;
                    
                    if (velAlongNormal < 0) {
                        float totalMass = particles[i].mass + particles[j].mass;
                        float impulse = -(1.0f + config.restitution) * velAlongNormal / totalMass;
                        
                        Vec2 impulseVec = normal * impulse;
                        forces[i] -= impulseVec * (particles[j].mass / config.deltaTime);
                        forces[j] += impulseVec * (particles[i].mass / config.deltaTime);
                    }
                    
                    float separationForce = overlap * 100.0f;
                    forces[i] -= normal * separationForce;
                    forces[j] += normal * separationForce;
                }
            }
        }
        
        for (int i = 0; i < count; i++) {
            Vec2 acceleration = forces[i] * (1.0f / particles[i].mass);
            particles[i].velocity += acceleration * config.deltaTime;
            particles[i].velocity = particles[i].velocity * config.friction;
            particles[i].position += particles[i].velocity * config.deltaTime;
            
            float radius = 5.0f;
            if (particles[i].position.x < radius) {
                particles[i].position.x = radius;
                particles[i].velocity.x *= -config.restitution;
            }
            if (particles[i].position.x > config.windowWidth - radius) {
                particles[i].position.x = config.windowWidth - radius;
                particles[i].velocity.x *= -config.restitution;
            }
            if (particles[i].position.y < radius) {
                particles[i].position.y = radius;
                particles[i].velocity.y *= -config.restitution;
            }
            if (particles[i].position.y > config.windowHeight - radius) {
                particles[i].position.y = config.windowHeight - radius;
                particles[i].velocity.y *= -config.restitution;
            }
        }
    }
};
