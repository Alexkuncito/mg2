#ifndef GESTORRECURSOS_HPP
#define GESTORRECURSOS_HPP

#include <vector>
#include <memory>
#include "recurso.hpp" 
#include "MaterialRecurso.hpp"
#include "MallaRecurso.hpp"
#include "ShaderRecurso.hpp"
#include "TexturaRecurso.hpp"
#include <cstring>
#include <iostream>
#include <unordered_map>

class TGestorRecursos {
private:
    std::unordered_map<std::string, std::unique_ptr<Recurso>> recursos;
public:
    // Método para obtener un recurso
    Recurso* getRecurso(const std::string& ruta);

    // En gestorrecursos.hpp
    void add(std::unique_ptr<Recurso> recurso);

    // Para imprimir los recursos en el gestor (para depuración)
    void ImprimirRecursos() const;

    void cargarMateriales();
};

#endif
