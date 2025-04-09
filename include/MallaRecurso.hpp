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
                 std::optional<std::reference_wrapper<const Textura>> textura = std::nullopt, int val = 0)
        : Recurso(fichero.getRuta(), val),
          malla(fichero, textura, val) {}

    Mesh* returnMalla() {
        return &malla;
    }
};

#endif
