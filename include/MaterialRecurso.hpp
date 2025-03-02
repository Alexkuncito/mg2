#ifndef RECURSOMATERIAL_HPP
#define RECURSOMATERIAL_HPP

#include "recurso.hpp"
#include "Material.hpp"

struct RecursoMaterial : public Recurso {
private:
    TMaterial material;

public:
    // Constructor que inicializa nombre y material
    RecursoMaterial(const char* nombre, const TMaterial& mat)
        : Recurso(nombre), material(mat) {}

    // Destructor
    ~RecursoMaterial() {}

    // Método para obtener el material
    const TMaterial& GetMaterial() const {
        return material;
    }

    // Método para establecer un nuevo material
    void SetMaterial(const TMaterial& nuevoMaterial) {
        material = nuevoMaterial;
    }

    // Método para aplicar el material a un shader
    void AplicarMaterial(Shader* shader) {
        material.SetMaterial(shader);
    }
};

#endif