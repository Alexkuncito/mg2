#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <glad.h>
#include "Fichero.hpp"
#include "Textura.hpp"
#include <optional>
#include <iostream> // Asegura que std::cerr esté disponible

class Mesh {
public:
    Mesh(const Fichero& fichero, std::optional<std::reference_wrapper<const Textura>> textura = std::nullopt, int val = 0);
    ~Mesh();

    void draw(Shader* shader) const;

private:
    unsigned int VAO, VBO, EBO;
    size_t indexCount;
    std::optional<std::reference_wrapper<const Textura>> textura;
    TMaterial material;
};

#endif
