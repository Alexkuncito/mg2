#ifndef GESTORRECURSOS_HPP
#define GESTORRECURSOS_HPP

#include <vector>
#include "recurso.hpp" 
#include "MaterialRecurso.hpp"
#include <cstring>
#include <iostream>

class TGestorRecursos {
private:
    std::vector<Recurso*> recursos;

public:
    // Destructor para liberar recursos
    ~TGestorRecursos();

    // Método para obtener un recurso
    Recurso* getRecurso(const char* nombre);

    // Método para añadir un recurso al gestor
    void add(Recurso* recurso);

    // Para imprimir los recursos en el gestor (para depuración)
    void ImprimirRecursos() const;
};

#endif
