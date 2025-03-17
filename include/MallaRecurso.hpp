#ifndef RECURSOMALLA_HPP
#define RECURSOMALLA_HPP

#include "recurso.hpp"
#include "Fichero.hpp"
#include "Mesh.hpp"
#include <optional>

struct RecursoMalla : public Recurso {
private:
    Mesh malla;

public:
    // Constructor de RecursoMalla que obtiene la ruta de Fichero y pasa a Recurso
    RecursoMalla(const Fichero& fichero, 
                 std::optional<std::reference_wrapper<const Textura>> textura = std::nullopt, 
                 std::optional<TMaterial> material = std::nullopt)
        : Recurso(fichero.getRuta()),
          malla(fichero, textura, material) {}

    // Función para dibujar la malla
    void Dibujar() const {
        malla.draw();
    }

    // Función para establecer material en un shader
    void AplicarMaterial(Shader* shader) {
        malla.setMat(shader);
    }

    std::shared_ptr<Mesh> returnMalla() {
        return std::make_shared<Mesh>(malla);
    }
};

#endif
