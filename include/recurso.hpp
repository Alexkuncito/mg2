#ifndef RECURSO_HPP
#define RECURSO_HPP

#include <cstring> // Para strlen y strcpy
#include <iostream> // Para imprimir en consola

struct Recurso {
private:
    char* nombre;
public:
    Recurso() : nombre(nullptr) {}
    virtual ~Recurso() {
        delete[] nombre;
    }

    const char* GetNombre() const {
        return nombre;
    }

    void SetNombre(const char* nuevoNombre) {
        if (nuevoNombre != nombre) {  // Comprobamos si el nuevo nombre es distinto
            if (nombre) {
                delete[] nombre; // Liberamos la memoria anterior
            }
            if (nuevoNombre) {
                nombre = new char[strlen(nuevoNombre) + 1]; // +1 para el carácter nulo '\0'
                strcpy(nombre, nuevoNombre);
            } else {
                nombre = nullptr;
            }
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

