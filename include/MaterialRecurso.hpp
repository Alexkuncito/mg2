#ifndef RECURSOMATERIAL_HPP
#define RECURSOMATERIAL_HPP

#include "recurso.hpp"
#include "Material.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

struct RecursoMaterial : public Recurso {
private:
    TMaterial mat;

public:
    // Constructor con valores por defecto
    RecursoMaterial() : Recurso(""), mat(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), 0.0f) {}

    // Constructor con valores
    RecursoMaterial(const glm::vec3& a, const glm::vec3& d, const glm::vec3& sp, float sh) 
        : Recurso(""), mat(a, d, sp, sh) {}

    // Constructor con ruta de fichero
    RecursoMaterial(const std::string& rutaFichero) : Recurso(rutaFichero), mat(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), 0.0f) {
        cargarFichero(rutaFichero);
    }

    // Retorna una copia del material
    TMaterial returnMaterial() const {
        return mat;
    }

    // Cargar material desde archivo
    bool cargarFichero(const std::string& rutaFichero) {
        std::ifstream archivo(rutaFichero);
        if (!archivo.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo " << rutaFichero << std::endl;
            return false;
        }

        crearNombre(rutaFichero);

        // Leer propiedades del material
        glm::vec3 ambient, diffuse, specular;
        float shininess = 0.0f;

        std::string linea;
        while (std::getline(archivo, linea)) {
            std::istringstream ss(linea);
            std::string clave;
            ss >> clave;

            if (clave == "ambient") {
                ss >> ambient.x >> ambient.y >> ambient.z;
            } else if (clave == "diffuse") {
                ss >> diffuse.x >> diffuse.y >> diffuse.z;
            } else if (clave == "specular") {
                ss >> specular.x >> specular.y >> specular.z;
            } else if (clave == "shininess") {
                ss >> shininess;
            }
        }

        archivo.close();

        // Asignar los valores leídos al material
        mat = TMaterial(ambient, diffuse, specular, shininess);
        return true;
    }

    TMaterial* returnMaterial() {
        return &mat;
    }
};

#endif
