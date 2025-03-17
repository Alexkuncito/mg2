#ifndef GESTORRECURSOS_HPP
#define GESTORRECURSOS_HPP

#include <vector>
#include <memory>
#include "recurso.hpp" 
#include "MaterialRecurso.hpp"
#include "MallaRecurso.hpp"
#include <cstring>
#include <iostream>

class TGestorRecursos {
private:
    std::vector<std::shared_ptr<Recurso>> recursos;

public:
    // Método para obtener un recurso
    std::shared_ptr<Recurso> getRecurso(const std::string& ruta);

    // Método para añadir un recurso al gestor
    void add(std::shared_ptr<Recurso> recurso);

    // Para imprimir los recursos en el gestor (para depuración)
    void ImprimirRecursos() const;

    void cargarMateriales();
};

#endif
