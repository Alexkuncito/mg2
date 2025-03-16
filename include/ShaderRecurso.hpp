#ifndef RECURSOSHADER_HPP
#define RECURSOSHADER_HPP

#include "recurso.hpp"
#include "Shader.hpp"
#include <string>

struct RecursoShader : public Recurso {
private:
    Shader shader;

public:
    // Constructor con nombre y rutas de los shaders
    RecursoShader(const std::string& rutaVertex, const std::string& rutaFragment)
        : Recurso(rutaVertex), shader(rutaVertex, rutaFragment) {}

    // Activa el shader
    void Usar() const {
        shader.use();
    }

    // Obtiene una referencia al shader
    Shader& ObtenerShader() {
        return shader;
    }

    // Obtiene una referencia constante al shader
    const Shader& ObtenerShader() const {
        return shader;
    }
};

#endif
