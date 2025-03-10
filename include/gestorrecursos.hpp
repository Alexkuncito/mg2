#ifndef GESTORRECURSOS_HPP
#define GESTORRECURSOS_HPP

#include <vector>
#include "recurso.hpp" 
#include "MaterialRecurso.hpp"
#include "MallaRecurso.hpp"
#include <cstring>
#include <iostream>

class TGestorRecursos {
private:
    std::vector<Recurso*> recursos;

public:
    // Método para obtener un recurso
    Recurso* getRecurso(const char* nombre);


    RecursoMalla* getRecursoMalla(const Fichero& fichero, 
                 std::optional<std::reference_wrapper<const Textura>> textura = std::nullopt, 
                 std::optional<TMaterial> material = std::nullopt);

    // Método para añadir un recurso al gestor
    void add(Recurso* recurso);

    // Para imprimir los recursos en el gestor (para depuración)
    void ImprimirRecursos() const;

    void cargarMateriales();
};

#endif
