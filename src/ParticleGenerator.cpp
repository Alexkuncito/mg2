#include "ParticleGenerator.hpp"
#include <cstdlib>

// Constructor
ParticleGenerator::ParticleGenerator(Shader* shader, Textura* texture, unsigned int amount)
    : shader(shader), texture(texture), amount(amount)
{
    this->init();
}

// Inicializa el VAO para representar una partícula como un quad
void ParticleGenerator::init()
{
    // Configura el quad que se usará para representar cada partícula
    unsigned int VBO;
    float particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    // Inicializa todas las partículas
    for (unsigned int i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

// Actualiza las partículas vivas y genera nuevas
void ParticleGenerator::Update(float dt, glm::vec2 position, glm::vec2 velocity, unsigned int newParticles, glm::vec2 offset)
{
    // Añade nuevas partículas
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unused = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unused], position, velocity, offset);
    }

    // Actualiza todas las partículas
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle &p = this->particles[i];
        p.Life -= dt;
        if (p.Life > 0.0f)
        {
            p.Position -= p.Velocity * dt;
            p.Color.a -= dt * 2.5f;
        }
    }
}

// Renderiza todas las partículas vivas
void ParticleGenerator::Draw()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader->use();

    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f); // ajusta a tu viewport
    shader->setMat4("projection", projection);

    for (Particle &particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            shader->setVec2("offset", particle.Position);
            shader->setVec4("color", particle.Color);
            texture->bind();

            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
}


// Busca la primera partícula muerta (Life <= 0.0f)
unsigned int ParticleGenerator::firstUnusedParticle()
{
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    lastUsedParticle = 0;
    return 0;
}

// Reinicia una partícula con nuevos valores
void ParticleGenerator::respawnParticle(Particle &particle, glm::vec2 position, glm::vec2 velocity, glm::vec2 offset)
{
    float randomX = static_cast<float>(rand() % 800);
    float randomY = static_cast<float>(rand() % 600);
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.Position = position + glm::vec2(randomX, randomY) + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = velocity * 0.1f;
}
