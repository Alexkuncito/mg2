#include "../include/gestorentidad.hpp"
#include <filesystem>

// Método para obtener una entidad por su ruta
MGEntity* TGestorEntidades::getEntidad(const std::string& ruta) {
    std::string nombreGenerado = MGEntity().crearNombre(ruta); // Suponiendo que MGEntity tiene crearNombre()

    auto it = Entidades.find(nombreGenerado);
    if (it != Entidades.end()) {
        return it->second.get();
    } else {
        return nullptr;
    }
}

// Método para obtener una entidad por su ruta + valor
MGEntity* TGestorEntidades::getEntidad(const std::string& ruta, int val) {
    std::string nombreGenerado = MGEntity().crearNombre(ruta, val); // Suponiendo sobrecarga
    auto it = Entidades.find(nombreGenerado);
    if (it != Entidades.end()) {
        return it->second.get();
    } else {
        return nullptr;
    }
}

// Método para agregar una entidad al gestor
void TGestorEntidades::add(std::unique_ptr<MGEntity> entidad) {
    std::string nombreEntidad = entidad->GetNombre();  // Suponiendo que MGEntity tiene GetNombre()
    Entidades[nombreEntidad] = std::move(entidad);
}

// Método para imprimir entidades (para depuración)
void TGestorEntidades::ImprimirEntidades() const {
    for (const auto& par : Entidades) {
        std::cout << "Entidad: " << par.first << std::endl;
    }
}


