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
    // Constructor por defecto
    RecursoMaterial() : mat(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), 0.0f) {}

    // Constructor con valores
    RecursoMaterial(const glm::vec3& a, const glm::vec3& d, const glm::vec3& sp, float sh) 
        : mat(a, d, sp, sh) {}

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

        // Obtener nombre del archivo y validar formato
        std::string nombreArchivo = rutaFichero.substr(rutaFichero.find_last_of("/") + 1);
        if (nombreArchivo.substr(0, 8) != "material" || nombreArchivo.find(".") == std::string::npos) {
            std::cerr << "Error: Formato de nombre de archivo incorrecto." << std::endl;
            return false;
        }

        size_t pos = nombreArchivo.find(".");
        SetNombre(nombreArchivo.substr(8, pos - 8).c_str()); 

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
};

#endif

