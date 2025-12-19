#include <cmath>
#include <random>

struct Vec2 {
    float x, y;
    
    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}
    
    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }
    
    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }
    
    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }
    
    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    float length() const {
        return std::sqrt(x * x + y * y);
    }
    
    float lengthSquared() const {
        return x * x + y * y;
    }
    
    Vec2 normalized() const {
        float len = length();
        if (len > 0.0001f) {
            return Vec2(x / len, y / len);
        }
        return Vec2(0, 0);
    }
};

struct Particle {
    Vec2 position;
    Vec2 velocity;
    float mass;
    
    Particle() : mass(1.0f) {}
    Particle(Vec2 pos, Vec2 vel, float m) : position(pos), velocity(vel), mass(m) {}
};

void initializeParticles(Particle* particles, int count, int width, int height) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posX(50.0f, width - 50.0f);
    std::uniform_real_distribution<float> posY(50.0f, height - 50.0f);
    std::uniform_real_distribution<float> vel(-50.0f, 50.0f);
    std::uniform_real_distribution<float> mass(0.5f, 2.0f);
    
    for (int i = 0; i < count; i++) {
        particles[i].position = Vec2(posX(gen), posY(gen));
        particles[i].velocity = Vec2(vel(gen), vel(gen));
        particles[i].mass = mass(gen);
    }
}