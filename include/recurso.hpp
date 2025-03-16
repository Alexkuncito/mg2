#ifndef RECURSO_HPP
#define RECURSO_HPP

#include <cstring> // Para strlen y strcpy
#include <iostream> // Para imprimir en consola
#include <filesystem>

namespace fs = std::filesystem;

struct Recurso {
private:
    char* nombre;
public:
    Recurso(const std::string& ruta) : nombre(nullptr) {
        crearNombre(ruta);
    }
    
    virtual ~Recurso() {
        delete[] nombre;
    }

    const char* GetNombre() const {
        return nombre;
    }

    void SetNombre(const char* nuevoNombre) {
        delete[] nombre;
        if (nuevoNombre) {
            nombre = new char[strlen(nuevoNombre) + 1];
            strcpy(nombre, nuevoNombre);
        } else {
            nombre = new char[1]{'\0'}; // Asignar un string vacío en lugar de nullptr
        }
    }

    std::string crearNombre(const std::string& ruta) {
        fs::path pathObj(ruta);
        std::string carpetaContenedora = pathObj.parent_path().filename().string();

        std::string nombreArchivo = pathObj.stem().string();
        std::string nuevoNombre = "Recurso_" + carpetaContenedora + "_" + nombreArchivo;

        std::cout << "Ruta completa: " << ruta << std::endl;
        std::cout << "Carpeta contenedora: " << carpetaContenedora << std::endl;
        std::cout << "Nombre del archivo sin extensión: " << nombreArchivo << std::endl;
        std::cout << "Nombre generado: " << nuevoNombre << std::endl;

        SetNombre(nuevoNombre.c_str());
        return nuevoNombre;
    }


    // Función que imprime el nombre
    void ImprimirNombre() const {
        if (nombre) {
            std::cout << "Nombre: " << nombre << std::endl;
        } else {
            std::cout << "Nombre no asignado" << std::endl;
        }
    }
};

#endif

