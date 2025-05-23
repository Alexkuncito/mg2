#include "gestorrecursos.hpp"
#include <filesystem>

// Método para obtener un recurso por su nombre
Recurso* TGestorRecursos::getRecurso(const std::string& ruta) {
    // Generar el nombre del recurso a partir de la ruta
    // std::cout << "BUSCANDO RECURSO." << std::endl;

    std::string nombreGenerado = Recurso().crearNombre(ruta); // Crear el nombre del recurso basado en la ruta

    // Buscar el recurso en el mapa
    auto it = recursos.find(nombreGenerado);
    
    // Verificar si el recurso fue encontrado
    if(it != recursos.end()){
        // std::cout << "Recurso encontrado." << std::endl;
        return it->second.get(); // Retorna una referencia al recurso encontrado
    } else {
        // std::cout << "Recurso no encontrado." << std::endl;
        return nullptr; // Si no se encuentra, retorna nullptr
    }
}

// Método para obtener un recurso por su nombre
Recurso* TGestorRecursos::getRecurso(const std::string& ruta, int val) {
    // Generar el nombre del recurso a partir de la ruta
    // std::cout << "BUSCANDO RECURSO." << std::endl;

    std::string nombreGenerado = Recurso().crearNombre(ruta, val); // Crear el nombre del recurso basado en la ruta

    // Buscar el recurso en el mapa
    auto it = recursos.find(nombreGenerado);
    
    // Verificar si el recurso fue encontrado
    if(it != recursos.end()){
        // std::cout << "Recurso encontrado." << std::endl;
        return it->second.get(); // Retorna una referencia al recurso encontrado
    } else {
        // std::cout << "Recurso no encontrado." << std::endl;
        return nullptr; // Si no se encuentra, retorna nullptr
    }
}

    void TGestorRecursos::add(std::unique_ptr<Recurso> recurso) {
        std::string nombreRecurso = recurso->GetNombre();
        std::cout << nombreRecurso << std::endl;
        recursos[nombreRecurso] = std::move(recurso);
    }

void TGestorRecursos::ImprimirRecursos() const {
    for (const auto& par : recursos) {
        std::cout << "Recurso: " << par.first << std::endl;  // Imprime la clave (nombre)
    }
}
