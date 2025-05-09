#ifndef PARTICLE_GENERATOR_3D_HPP
#define PARTICLE_GENERATOR_3D_HPP

#include <vector>
#include "glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Particle3D.hpp"
#include "Shader.hpp"
#include "Textura.hpp"

class ParticleGenerator3D {
public:
    ParticleGenerator3D(Shader* shader, Textura* texture, unsigned int amount, float particleScale, float horizontal, float vertical, float profundidad, std::string form, std::string particleForm);

    void Update(float dt, glm::vec3 position, glm::vec3 velocity, unsigned int newParticles, glm::vec3 offset);
    void Draw(const glm::mat4& view);
    void Draw2(const glm::mat4& view);



private:
    std::vector<Particle3D> particles;
    Shader* shader;
    Textura* texture;
    unsigned int VAO, VBO;

    unsigned int edgeVAO, edgeVBO;

    unsigned int amount;


    float scale = 1.0f;
    float horizontal = 1.0f;
    float vertical = 1.0f;
    float profundidad = 1.0f;    
    std::string form = "cube";
    

    void init(std::string particleForm="cube");
    unsigned int firstUnusedParticle();
    void respawnParticle(Particle3D& particle, glm::vec3 position, glm::vec3 velocity, glm::vec3 offset);
    unsigned int lastUsedParticle = 0;
};

#endif // PARTICLE_GENERATOR_3D_HPP
