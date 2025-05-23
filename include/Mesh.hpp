#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <glad.h>
#include "Fichero.hpp"
#include "Textura.hpp"
#include <optional>
#include <iostream> 

class Mesh {
public:
    Mesh(const Fichero& fichero, std::optional<std::reference_wrapper<const Textura>> textura = std::nullopt, int val = 0);
    ~Mesh();

    void draw(Shader* shader, std::optional<glm::vec4> col = std::nullopt) const;

    void setMaterial(const TMaterial& MAT){
        glm::vec3 ambient = MAT.getAmbient();
        glm::vec3 diffuse = MAT.getDiffuse();
        glm::vec3 specular = MAT.getSpecular();
        float shininess = MAT.getShininess();
    
        std::cout << "Ambient color: (" 
                  << ambient.x << ", " 
                  << ambient.y << ", " 
                  << ambient.z << ")\n";
    
        std::cout << "Diffuse color: (" 
                  << diffuse.x << ", " 
                  << diffuse.y << ", " 
                  << diffuse.z << ")\n";
    
        std::cout << "Specular color: (" 
                  << specular.x << ", " 
                  << specular.y << ", " 
                  << specular.z << ")\n";
    
        std::cout << "Shininess: " << shininess << "\n";
                material = MAT;
    }

private:
    unsigned int VAO, VBO, EBO;
    size_t indexCount;
    std::optional<std::reference_wrapper<const Textura>> textura;
    TMaterial material;
};

#endif
