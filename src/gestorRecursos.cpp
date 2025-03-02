#include "gestorrecursos.hpp"

// Destructor para liberar los recursos del vector
TGestorRecursos::~TGestorRecursos() {
    for (auto rec : recursos) {
        delete rec;
    }
}

// Método para obtener un recurso por su nombre
Recurso* TGestorRecursos::getRecurso(const char* nombre) {
    // Buscar el recurso en el vector
    for (auto rec : recursos) {
        if (strcmp(rec->GetNombre(), nombre) == 0) {
            return rec;  // Si lo encontramos, lo retornamos
        }
    }
    return nullptr;
}

// Método para añadir un recurso al gestor
void TGestorRecursos::add(Recurso* recurso) {
    recursos.push_back(recurso);  // Añadimos el recurso al vector
}

void TGestorRecursos::ImprimirRecursos() const {
    for (auto rec : recursos) {
        // Intentamos hacer cast a las clases derivadas (RecursoMaterial, etc.)
        if (dynamic_cast<RecursoMaterial*>(rec)) {
            std::cout << "Recurso: Material -> " << rec->GetNombre() << std::endl;
        }
        else {
            std::cout << "Recurso: Desconocido -> " << rec->GetNombre() << std::endl;
        }
    }
}