#ifndef RECURSO_HPP
#define RECURSO_HPP

#include <cstring> // Para strlen y strcpy
#include <iostream> // Para imprimir en consola

struct Recurso {
private:
    char* nombre;
public:
    Recurso(const char* inicialNombre = nullptr) : nombre(nullptr) {
        if (inicialNombre) {
            nombre = new char[strlen(inicialNombre) + 1];
            strcpy(nombre, inicialNombre);
        }
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

