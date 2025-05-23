#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/glm.hpp>
#include <assimp/material.h>
#include <iostream>
#include <glad.h>
#include "Shader.hpp"

struct TMaterial {
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    float opacity = 1.0f;
    bool existencia = false;

public:
    // Constructor por defecto
    TMaterial()
        : ambient(0.2f), diffuse(0.8f), specular(1.0f), shininess(32.0f) {}

    // Constructor con parámetros
    TMaterial(const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec, float shin)
        : ambient(amb), diffuse(diff), specular(spec), shininess(shin) {}

    // Constructor desde aiMaterial
    TMaterial(aiMaterial* mat) {
        aiColor3D color(0.f, 0.f, 0.f);
        aiString texPath;
    
        bool hasTexture = (mat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS);
        existencia = !hasTexture;
    
        if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_AMBIENT, color))
            ambient = glm::vec3(color.r, color.g, color.b);
        else
            ambient = glm::vec3(0.2f);
    
        if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
            diffuse = glm::vec3(color.r, color.g, color.b);
        } else {
            diffuse = glm::vec3(0.8f);
        }
    
        if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_SPECULAR, color))
            specular = glm::vec3(color.r, color.g, color.b);
        else
            specular = glm::vec3(1.0f);

        // Opacidad
        float opac = 1.0f;
        if (AI_SUCCESS == mat->Get(AI_MATKEY_OPACITY, opac)) {
            opacity = opac;
        } else {
            opacity = 1.0f;
        }
    
        float shin = 0.0f;
        if (AI_SUCCESS == mat->Get(AI_MATKEY_SHININESS, shin)) {
            shininess = shin;
        } else {
            shininess = 32.0f;
        }
    

    }
    
    bool existe() const { return existencia; }

    // Enviar al shader
    void SetMaterial(Shader* shader) const {
        static TMaterial anteriorMaterial;
        static Shader* anteriorShader = nullptr;
        //fallback
    
        if (shader == nullptr) {
            std::cerr << "Error: Shader es nullptr en SetMaterial()" << std::endl;
            return;
        }
    
        auto approxEqual = [](const glm::vec3& a, const glm::vec3& b, float epsilon = 0.001f) {
            return glm::all(glm::epsilonEqual(a, b, epsilon));
        };
    
        if (shader == anteriorShader &&
            approxEqual(anteriorMaterial.ambient, ambient) &&
            approxEqual(anteriorMaterial.diffuse, diffuse) &&
            approxEqual(anteriorMaterial.specular, specular) &&
            std::abs(anteriorMaterial.shininess - shininess) < 0.001f &&
            std::abs(anteriorMaterial.opacity - opacity) < 0.001f) {
            return;
        }
    
        anteriorMaterial = *this;
        anteriorShader = shader;
    
        shader->use();
        shader->setVec3("material.ambient", ambient);
        shader->setVec3("material.diffuse", diffuse);
        shader->setVec3("material.specular", specular);
        shader->setFloat("material.shininess", shininess);
        shader->setFloat("material.opacity", opacity);

    }
    

    // Accesores (opcional)
    glm::vec3 getAmbient() const { return ambient; }
    glm::vec3 getDiffuse() const { return diffuse; }
    glm::vec3 getSpecular() const { return specular; }
    float getShininess() const { return shininess; }

    void printMaterial() {
        std::cout << "IMPRIMIENDO MATERIAL\n";
        std::cout << "AMBIENT: (" << ambient.r << ", " << ambient.g << ", " << ambient.b << ")\n";
        std::cout << "DIFFUSE: (" << diffuse.r << ", " << diffuse.g << ", " << diffuse.b << ")\n";
        std::cout << "SPECULAR: (" << specular.r << ", " << specular.g << ", " << specular.b << ")\n";
        std::cout << "SHININESS: " << shininess << std::endl;
    }
    
};

#endif
