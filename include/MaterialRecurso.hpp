#ifndef RECURSOMATERIAL_HPP
#define RECURSOMATERIAL_HPP

#include "recurso.hpp"
#include "Material.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

struct RecursoMaterial : public Recurso {
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

public:
    TMaterial returnMaterial(){
        return TMaterial(ambient, diffuse, specular, shininess);
    }

    bool cargarFichero(const std::string& rutaFichero) {
        std::ifstream archivo(rutaFichero);
        if (!archivo.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo " << rutaFichero << std::endl;
            return false;
        }


        
        std::string nombreArchivo = rutaFichero.substr(rutaFichero.find_last_of("/") + 1);
        if (nombreArchivo.substr(0, 8) != "material" || nombreArchivo.find(".") == std::string::npos) {
            std::cerr << "Error: Formato de nombre de archivo incorrecto." << std::endl;
            return false;
        }
        
        size_t pos = nombreArchivo.find(".");
        SetNombre(nombreArchivo.substr(8, pos - 8).c_str()); // Extrae el nombre del material y convierte a const char*

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
        return true;
    }
};

#endif
