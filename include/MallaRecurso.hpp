#ifndef RECURSOMALLA_HPP
#define RECURSOMALLA_HPP

#include "recurso.hpp"
#include "Mesh.hpp"

struct RecursoMalla : public Recurso {
    private:
        Mesh malla;
    public:
        RecursoMalla(const Fichero& fichero, 
                 std::optional<std::reference_wrapper<const Textura>> textura = std::nullopt, 
                 std::optional<TMaterial> material = std::nullopt) 
        : malla(fichero, textura, material) {}

    // Función para dibujar la malla
    void Dibujar() const {
        malla.draw();
    }

    // Función para establecer material en un shader
    void AplicarMaterial(Shader* shader) {
        malla.setMat(shader);
    }
};

#endif