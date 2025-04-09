#ifndef RECURSO_HPP
#define RECURSO_HPP

#include <iostream> // Para imprimir en consola
#include <filesystem>

namespace fs = std::filesystem;

struct Recurso {
private:
    std::string nombre;
public:
    Recurso() : nombre("") {}

    Recurso(const std::string& ruta, int val = 0) {
        std::cout << "Recurso TIPO 2" << std::endl;
        crearNombre(ruta, val);
    }

    virtual ~Recurso() = default;

    const std::string& GetNombre() const {
        return nombre;
    }

    void SetNombre(const std::string& nuevoNombre) {
        nombre = nuevoNombre;
    }

    std::string crearNombre(const std::string& ruta, int val = 0) {
        fs::path pathObj(ruta);
        std::string carpetaContenedora = pathObj.parent_path().filename().string();

        std::string nombreArchivo = pathObj.stem().string();
        std::string nuevoNombre = "Recurso_" + carpetaContenedora + "_" + nombreArchivo + "_" + std::to_string(val);

        // std::cout << "Ruta completa: " << ruta << std::endl;
        // std::cout << "Carpeta contenedora: " << carpetaContenedora << std::endl;
        // std::cout << "Nombre del archivo sin extensión: " << nombreArchivo << std::endl;
        // std::cout << "Nombre generado: " << nuevoNombre << std::endl;

        SetNombre(nuevoNombre);
        return nuevoNombre;
    }

    // Función que imprime el nombre
    void ImprimirNombre() const {
        if (!nombre.empty()) {
            std::cout << "Nombre: " << nombre << std::endl;
        } else {
            std::cout << "Nombre no asignado" << std::endl;
        }
    }
};

#endif

