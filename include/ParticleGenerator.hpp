#ifndef PARTICLE_GENERATOR_HPP
#define PARTICLE_GENERATOR_HPP

#include <vector>
#include "glad.h"
#include "glm/glm.hpp"

#include "Particle.hpp"

#include "Shader.hpp"
#include "Textura.hpp"

class ParticleGenerator {
public:
    // Constructor
    ParticleGenerator(Shader* shader, Textura* texture, unsigned int amount);

    // Update all particles
    void Update(float dt, glm::vec2 position, glm::vec2 velocity, unsigned int newParticles, glm::vec2 offset);

    // Render all particles
    void Draw();

private:
    // State
    std::vector<Particle> particles;

    // Render state
    Shader* shader;
    Textura* texture;
    unsigned int VAO;
    unsigned int amount;


    // Helper functions
    void init();
    unsigned int firstUnusedParticle();
    void respawnParticle(Particle &particle, glm::vec2 position, glm::vec2 velocity,glm::vec2 offset);

    // Stores the index of the last used particle (for faster searching)
    unsigned int lastUsedParticle = 0;
};

#endif // PARTICLE_GENERATOR_HPP