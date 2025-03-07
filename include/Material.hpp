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

        GLint locAmbient = glGetUniformLocation(shader->getID(), "material.ambient");
        GLint locDiffuse = glGetUniformLocation(shader->getID(), "material.diffuse");
        GLint locSpecular = glGetUniformLocation(shader->getID(), "material.specular");
        GLint locShininess = glGetUniformLocation(shader->getID(), "material.shininess");

        if (locAmbient == -1 || locDiffuse == -1 || locSpecular == -1 || locShininess == -1) {
            std::cerr << "Error: Uniformes del material no encontrados en el shader" << std::endl;
            return;
        }

        shader->setVec3("material.ambient", ambient);
        shader->setVec3("material.diffuse", diffuse);
        shader->setVec3("material.specular", specular);
        shader->setFloat("material.shininess", shininess);
    }


};

#endif
