#include "ParticleGenerator3D.hpp"
#include <cstdlib>

ParticleGenerator3D::    ParticleGenerator3D(Shader* shader, Textura* texture, unsigned int amount, float particleScale, float horizontal, float vertical, float profundidad, std::string form, std::string particleForm="star")
    : shader(shader), texture(texture), amount(amount), scale(particleScale), horizontal(horizontal), vertical(vertical), profundidad(profundidad), form(form)
{
    this->init(particleForm);
}

void ParticleGenerator3D::init(std::string particleForm)
{
    float quads[] = {
        // Quad XY (frontal)
        -0.5,  0.5, 0,   0, 1,   0, 0, 1,
         0.5, -0.5, 0,   1, 0,   0, 0, 1,
        -0.5, -0.5, 0,   0, 0,   0, 0, 1,
    
        -0.5,  0.5, 0,   0, 1,   0, 0, 1,
         0.5,  0.5, 0,   1, 1,   0, 0, 1,
         0.5, -0.5, 0,   1, 0,   0, 0, 1,
    
        // Quad YZ (perfil)
         0,  0.5, -0.5,   0, 1,   1, 0, 0,
         0, -0.5,  0.5,   1, 0,   1, 0, 0,
         0, -0.5, -0.5,   0, 0,   1, 0, 0,
    
         0,  0.5, -0.5,   0, 1,   1, 0, 0,
         0,  0.5,  0.5,   1, 1,   1, 0, 0,
         0, -0.5,  0.5,   1, 0,   1, 0, 0,
    
        // Quad XZ (planta)
        -0.5, 0,  0.5,   0, 1,   0, 1, 0,
         0.5, 0, -0.5,   1, 0,   0, 1, 0,
        -0.5, 0, -0.5,   0, 0,   0, 1, 0,
    
        -0.5, 0,  0.5,   0, 1,   0, 1, 0,
         0.5, 0,  0.5,   1, 1,   0, 1, 0,
         0.5, 0, -0.5,   1, 0,   0, 1, 0,
    };
        // Coordenadas de los vértices para el "cube"
        float cube[] = {
            // Quad XY (frontal)
            -0.5,  0.5, 0.5,   0, 1,   0, 0, 1,
            0.5, -0.5, 0.5,   1, 0,   0, 0, 1,
            -0.5, -0.5, 0.5,   0, 0,   0, 0, 1,

            -0.5,  0.5, 0.5,   0, 1,   0, 0, 1,
            0.5,  0.5, 0.5,   1, 1,   0, 0, 1,
            0.5, -0.5, 0.5,   1, 0,   0, 0, 1,

            // Quad XY (trasera)
            -0.5,  0.5, -0.5,   1, 1,   0, 0, 1,
            0.5, -0.5, -0.5,   0, 0,   0, 0, 1,
            -0.5, -0.5, -0.5,   1, 0,   0, 0, 1,

            -0.5,  0.5, -0.5,   1, 1,   0, 0, 1,
            0.5,  0.5, -0.5,   0, 1,   0, 0, 1,
            0.5, -0.5, -0.5,   0, 0,   0, 0, 1,

            // Quad YZ (perfil der) 
            0.5,  0.5, -0.5,   1, 1,   1, 0, 0,
            0.5, -0.5,  0.5,   0, 0,   1, 0, 0,
            0.5, -0.5, -0.5,   1, 0,   1, 0, 0,

            0.5,  0.5, -0.5,   1, 1,   1, 0, 0,
            0.5,  0.5,  0.5,   0, 1,   1, 0, 0,
            0.5, -0.5,  0.5,   0, 0,   1, 0, 0,

            // Quad YZ (perfil izq)
            -0.5,  0.5, -0.5,   0, 1,   1, 0, 0,
            -0.5, -0.5,  0.5,   1, 0,   1, 0, 0,
            -0.5, -0.5, -0.5,   0, 0,   1, 0, 0,

            -0.5,  0.5, -0.5,   0, 1,   1, 0, 0,
            -0.5,  0.5,  0.5,   1, 1,   1, 0, 0,
            -0.5, -0.5,  0.5,   1, 0,   1, 0, 0,

            // Quad XZ (planta arriba)
            -0.5, 0.5,  0.5,   0, 0,   0, 1, 0,
            0.5, 0.5, -0.5,   1, 1,   0, 1, 0,
            -0.5, 0.5, -0.5,   0, 1,   0, 1, 0,

            -0.5, 0.5,  0.5,   0, 0,   0, 1, 0,
            0.5, 0.5,  0.5,   1, 0,   0, 1, 0,
            0.5, 0.5, -0.5,   1, 1,   0, 1, 0,


            // Quad XZ (planta abajo)
            -0.5, -0.5,  0.5,   0, 0,   0, 1, 0,
            0.5, -0.5, -0.5,   1, 1,   0, 1, 0,
            -0.5, -0.5, -0.5,   0, 1,   0, 1, 0,

            -0.5, -0.5,  0.5,   0, 0,   0, 1, 0,
            0.5, -0.5,  0.5,   1, 0,   0, 1, 0,
            0.5, -0.5, -0.5,   1, 1,   0, 1, 0,

        };
    
    if(particleForm == "cube") {
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &VBO);
    
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // aPos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // aTexCoord
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); // aNormal
        glEnableVertexAttribArray(2);
    
        glBindVertexArray(0);
    
        for (unsigned int i = 0; i < this->amount; ++i)
            this->particles.push_back(Particle3D());
    }
    else{
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quads), quads, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // aPos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // aTexCoord
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); // aNormal
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        for (unsigned int i = 0; i < this->amount; ++i)
            this->particles.push_back(Particle3D());
        }
}



void ParticleGenerator3D::Update(float dt, glm::vec3 position, glm::vec3 velocity, unsigned int newParticles, glm::vec3 offset)
{
    for (unsigned int i = 0; i < newParticles; ++i) {
        int unused = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unused], position, velocity, offset);
    }

    for (unsigned int i = 0; i < this->amount; ++i) {
        Particle3D& p = this->particles[i];

        // Reducir la vida de la partícula con base en el tiempo delta
        p.Life -= dt;  // Descontar tiempo según el delta time
        if (p.Life > 0.0f) {
            
            p.Position.x += velocity.x * dt + (rand() % 2 == 0 ? -1.0f : 1.0f) * 0.1f;
            p.Position.y += velocity.y * dt + (rand() % 2 == 0 ? -1.0f : 1.0f) * 0.1f;
            p.Position.z += velocity.z * dt + (rand() % 2 == 0 ? -1.0f : 1.0f) * 0.1f;

            // Calcular el color basado en la vida restante
            float lifeRatio = p.Life / pLIFE3D;  // Ratio de vida para calcular transparencia
            p.Color.a = lifeRatio;
        }
    }
}


void ParticleGenerator3D::Draw(const glm::mat4& view)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader->use();
    shader->setMat4("view", view);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f); // ajusta a tu viewport
    shader->setMat4("projection", projection);

    for (Particle3D& particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, particle.Position);
            model = glm::scale(model, glm::vec3(scale)); // escala configurable

            shader->setMat4("model", model);
            shader->setVec4("color", particle.Color);
            texture->bind();

            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 54);
            glBindVertexArray(0);
        }
    }
}

unsigned int ParticleGenerator3D::firstUnusedParticle()
{
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
        if (particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator3D::respawnParticle(Particle3D& particle, glm::vec3 position, glm::vec3 velocity, glm::vec3 offset)
{
    if (form == "cube") {
        // Generación de las posiciones de las partículas dentro de un cubo usando horizontal y vertical
        float randomX = static_cast<float>(rand() % 1000) / 1000.0f * horizontal;  // horizontal es el rango para X
        float randomY = static_cast<float>(rand() % 1000) / 1000.0f * vertical;   // vertical es el rango para Y
        float randomZ = static_cast<float>(rand() % 1000) / 1000.0f * profundidad; // profundidad sigue siendo el rango para Z
        
        particle.Position = position + glm::vec3(randomX, randomY, randomZ) + offset;
    }
    else if (form == "sphere") {
        // Generación de las posiciones de las partículas dentro de un elipsoide (esfera con diferentes radios)
        float radiusX = horizontal;   // Radio para el eje X y Z (horizontal)
        float radiusY = vertical;     // Radio para el eje Y (vertical)
        float radiusZ = profundidad;  // Radio para el eje Z (profundidad)

        // Ángulos en coordenadas esféricas
        float theta = static_cast<float>(rand() % 1000) / 1000.0f * 2.0f * glm::pi<float>();  // Ángulo azimutal
        float phi = static_cast<float>(rand() % 1000) / 1000.0f * glm::pi<float>();  // Ángulo polar

        // Convertir coordenadas esféricas a cartesianas y escalar para ajustarse al elipsoide
        float x = radiusX * glm::sin(phi) * glm::cos(theta);  // Radio en el eje X
        float y = radiusY * glm::sin(phi) * glm::sin(theta);  // Radio en el eje Y
        float z = radiusZ * glm::cos(phi);                    // Radio en el eje Z

        particle.Position = position + glm::vec3(x, y, z) + offset;
    }
    else if (form == "pyramid") {
        float baseWidth = horizontal;    // Ancho de la base
        float baseDepth = profundidad;   // Profundidad de la base
        float height = vertical;         // Altura de la pirámide
        
        // Altura aleatoria dentro de la pirámide
        float randomHeight = static_cast<float>(rand() % 1000) / 1000.0f * height;
        
        // Ajustar el tamaño de la base de la pirámide según la altura
        float scaleFactor = 1.0f - randomHeight / height;  // A medida que subimos, la base se hace más pequeña
        
        // Posiciones aleatorias dentro de la base escalada
        float randomX = static_cast<float>(rand() % 1000) / 1000.0f * (baseWidth * scaleFactor);
        float randomZ = static_cast<float>(rand() % 1000) / 1000.0f * (baseDepth * scaleFactor);
        
        // Distribuir las partículas dentro de la pirámide
        particle.Position = position + glm::vec3(randomX - (baseWidth * scaleFactor / 2), randomHeight, randomZ - (baseDepth * scaleFactor / 2)) + offset;
    }
    else if (form == "cone") {
    float baseWidth = horizontal;   // Ancho de la base (eje X)
    float baseDepth = profundidad;  // Profundidad de la base (eje Z)
    float height = vertical;        // Altura del cono (eje Y)
    
    // Altura aleatoria dentro del rango de la pirámide
    float randomHeight = static_cast<float>(rand() % 1000) / 1000.0f * height;
    
    // Ajustar el tamaño de la base según la altura de la partícula (proporcional al nivel de la altura)
    float scaleFactorX = 1.0f - randomHeight / height;  // Escalado en el eje X
    float scaleFactorZ = 1.0f - randomHeight / height;  // Escalado en el eje Z
    
    // Posiciones aleatorias dentro de la base escalada en X y Z
    float randomX = static_cast<float>(rand() % 1000) / 1000.0f * (baseWidth * scaleFactorX);
    float randomZ = static_cast<float>(rand() % 1000) / 1000.0f * (baseDepth * scaleFactorZ);
    
    // Distribuir las partículas dentro del cono, ajustando por la escala y la altura
    particle.Position = position + glm::vec3(randomX - (baseWidth * scaleFactorX / 2), randomHeight, randomZ - (baseDepth * scaleFactorZ / 2)) + offset;
}
else if (form == "cone") {
    float radius = horizontal;  // El radio de la base del cono
    float height = vertical;    // La altura del cono
    
    // Ángulo azimutal (en coordenadas cilíndricas)
    float theta = static_cast<float>(rand() % 1000) / 1000.0f * 2.0f * glm::pi<float>();  // Ángulo azimutal
    // Radio ajustado según la altura (para crear una forma cónica)
    float randomHeight = static_cast<float>(rand() % 1000) / 1000.0f * height;  // Altura aleatoria
    
    // El radio en la base del cono se ajusta dependiendo de la altura
    float adjustedRadius = radius * (1.0f - randomHeight / height);
    
    // Coordenadas cartesianas en base a las coordenadas cilíndricas
    float x = adjustedRadius * glm::cos(theta);
    float y = randomHeight;  // La altura es simplemente el valor aleatorio de la altura
    float z = adjustedRadius * glm::sin(theta);
    
    particle.Position = position + glm::vec3(x, y, z) + offset;
}

    // Generar color aleatorio
    float rColor = 0.5f + static_cast<float>(rand() % 100) / 100.0f; // Aleatorio entre 0.5 y 1.0
    particle.Color = glm::vec4(rColor, rColor, rColor, 0.7f);


    // Configuración de vida y velocidad
    particle.Life = pLIFE3D;
    particle.Velocity = velocity; // Ajuste de velocidad si es necesario
}




void ParticleGenerator3D::Draw2(const glm::mat4& view)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader->use();
    
    shader->setMat4("view", view);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    shader->setMat4("projection", projection);

    // Creamos una sola partícula manualmente en el centro (0,0,0)
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f)); // Escala de la partícula

    shader->setMat4("model", model);
    shader->setVec4("color", glm::vec4(1.0f)); // Color blanco, completamente opaco
    texture->bind();

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 54); // 3 quads * 2 triángulos * 3 vértices = 54
    glBindVertexArray(0);
    
    
}

