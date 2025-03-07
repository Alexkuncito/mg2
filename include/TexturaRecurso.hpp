#ifndef RECURSOTEXTURA_HPP
#define RECURSOTEXTURA_HPP

#include "recurso.hpp"
#include "Textura.hpp"

struct RecursoTextura : public Recurso {
private:
    Textura textura;

public:
    // Constructor con nombre y ruta de textura
    RecursoTextura(const char* nombre, const std::string& rutaTextura)
        : Recurso(nombre), textura(rutaTextura) {}

    // Enlaza la textura para su uso
    void Activar() const {
        textura.bind();
    }

    // Desenlaza la textura
    void Desactivar() const {
        textura.unbind();
    }

    // Devuelve una referencia a la textura
    const Textura& ObtenerTextura() const {
        return textura;
    }
};

#endif
