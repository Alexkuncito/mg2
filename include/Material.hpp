#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/glm.hpp>
#include "glad.h"
#include "Shader.hpp"

struct TMaterial{
    private:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
    public:
        // Constructor
        TMaterial(const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec, float shin)
            : ambient(amb), diffuse(diff), specular(spec), shininess(shin) {}

        // Destructor
        ~TMaterial() {}

        
        void SetMaterial(Shader* shader) {
        if (!shader) {
            std::cerr << "Error: Shader es nullptr en SetMaterial()" << std::endl;
            return;
        }

        shader->use(); // Asegurar que el shader está activo

        if (glGetUniformLocation(shader->getID(), "material.ambient") == -1){
        std::cerr << "Error: Uniform 'material.ambient' no encontrado en el shader" << std::endl;
        }

        if (glGetUniformLocation(shader->getID(), "material.diffuse") == -1) {
        std::cerr << "Error: Uniform 'material.diffuse' no encontrado en el shader" << std::endl;
        }
        if (glGetUniformLocation(shader->getID(), "material.specular") == -1) {
            std::cerr << "Error: Uniform 'material.specular' no encontrado en el shader" << std::endl;
        }
        if (glGetUniformLocation(shader->getID(), "material.shininess") == -1) {
            std::cerr << "Error: Uniform 'material.shininess' no encontrado en el shader" << std::endl;
        }
        
        // Si todo está bien, asignar valores
        shader->setVec3("material.ambient", ambient);
        shader->setVec3("material.diffuse", diffuse);
        shader->setVec3("material.specular", specular);
        shader->setFloat("material.shininess", shininess);
    }


};

#endif
