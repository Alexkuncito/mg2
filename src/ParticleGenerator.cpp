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
        -5.0f, 5.0f, 0.0f, 1.0f,
        5.0f, -5.0f, 1.0f, 0.0f,
        -5.0f, -5.0f, 0.0f, 0.0f, 

        -5.0f, 5.0f, 0.0f, 1.0f,
        5.0f, 5.0f, 1.0f, 1.0f,
        5.0f, -5.0f, 1.0f, 0.0f
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
            /// Rebote en los límites de X, teniendo en cuenta el tamaño de la partícula
            if (p.Position.x <= 50.0f || p.Position.x >= 350.0f) {
                p.Velocity.x *= -1.0f;
            }

            // Movimiento en el eje X
            p.Position.x += p.Velocity.x * 25.f;

            // Evitar que la partícula se salga del límite de 400, considerando su tamaño
            if (p.Position.x < 50.0f) {
                p.Position.x = 50.0f;  // Ajusta la posición al límite izquierdo (considerando el tamaño de la partícula)
            }
            if (p.Position.x > 350.0f) {
                p.Position.x = 350.0f;  // Ajusta la posición al límite derecho (considerando el tamaño de la partícula)
            }

            float lifeRatio = p.Life / pLIFE;
            p.Color.a = lifeRatio * lifeRatio * 0.5;        }
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
    float randomX = (rand() % 2 == 0) ? 50.0f : 350.0f;
    float randomY = static_cast<float>(rand() % 600);
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.Position = position  +glm::vec2(randomX, randomY)+ offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 0.5f);
    particle.Life = pLIFE;
    float randomDir = (rand() % 2 == 0) ? 1.0f : -1.0f; // dirección aleatoria: 1 o -1
    float randomSpeed = 0.05f + static_cast<float>(rand() % 100) / 1000.0f; // entre 0.05 y 0.15
    particle.Velocity = glm::vec2(velocity.x * randomDir * randomSpeed, velocity.y * randomSpeed);
    }
