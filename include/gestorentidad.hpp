#ifndef GESTORENTIDADES_HPP
#define GESTORENTIDADES_HPP

#include <vector>
#include <memory>
#include "recurso.hpp" 
#include "entidad.hpp"
#include <cstring>
#include <iostream>
#include <unordered_map>

class TGestorEntidades {
private:
    std::unordered_map<std::string, std::unique_ptr<MGEntity>> Entidades;
public:

    TGestorEntidades(){
        Entidades.reserve(1000);
    }

    // Método para obtener un recurso
    MGEntity* getEntidad(const std::string& ruta);
    MGEntity* getEntidad(const std::string& ruta, int val);

    // En gestorrecursos.hpp
    void add(std::unique_ptr<MGEntity> entidad);

    // Para imprimir los recursos en el gestor (para depuración)
    void ImprimirEntidades() const;


    std::vector<MGMesh*> getCompEntidades(const std::string& rutaBase) {
        std::vector<MGMesh*> resultado;
    
        // Obtener nombre base sin el índice
        std::string nombreBase = MGEntity().crearNombre(rutaBase); // sin número
    
        // Quitar el sufijo "_0" o similar si viene de crearNombre()
        std::string baseSinIndice = nombreBase;
        size_t pos = nombreBase.find_last_of('_');
        if (pos != std::string::npos && std::all_of(nombreBase.begin() + pos + 1, nombreBase.end(), ::isdigit)) {
            baseSinIndice = nombreBase.substr(0, pos);
        }
    
        // Buscar todas las entidades que coincidan con el patrón base + "_" + número
        for (const auto& par : Entidades) {
            const std::string& nombre = par.first;
            if (nombre.rfind(baseSinIndice + "_", 0) == 0) { // comienza con baseSinIndice_
                MGEntity* entidad = par.second.get();
                if (entidad) {
                    MGMesh* mesh = dynamic_cast<MGMesh*>(entidad);
                    if (mesh) {
                        resultado.push_back(mesh);
                    }
                }
            }
        }
    
        return resultado;
    }
};

#endif