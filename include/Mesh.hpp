#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <glad.h>
#include "Fichero.hpp"
#include "Textura.hpp"
#include "Material.hpp"
#include <optional>
#include <iostream> // Asegura que std::cerr esté disponible

class Mesh {
public:
    Mesh(const Fichero& fichero, std::optional<std::reference_wrapper<const Textura>> textura = std::nullopt, std::optional<TMaterial> material = std::nullopt)
        : textura(textura), material(material) {  // Copiamos el fichero

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        if (!fichero.obtenerDatos(vertices, indices)) { 
            std::cerr << "Error: No se pudieron obtener los datos del fichero." << std::endl;
            return;
        }

        indexCount = indices.size();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    ~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void setMat(Shader* s) {
        if (material) {
            material->SetMaterial(s);
        }
    }

    void draw() const {
        if (textura) {
            textura->get().bind();
        } else {
            glBindTexture(GL_TEXTURE_2D, 0); // Desactiva cualquier textura activa
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

        if (textura) {
            textura->get().unbind();
        } else {
            glBindTexture(GL_TEXTURE_2D, 0); // Asegurar que ninguna textura quede activa
        }

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL Error: " << error << std::endl;
        }
    }


private:
    unsigned int VAO, VBO, EBO;
    size_t indexCount;
    std::optional<std::reference_wrapper<const Textura>> textura;
    std::optional<TMaterial> material;
};

#endif
