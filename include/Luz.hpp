#ifndef LUZ_HPP
#define LUZ_HPP

#include <glm/glm.hpp>
#include "Shader.hpp"

class Luz {
public:
    glm::vec3 posicion;
    glm::vec3 color;
    float intensidad;

    // Constructor
    Luz(const glm::vec3& pos, const glm::vec3& col, float inten);

    // Métodos de la clase
    glm::vec3 getPos();
    void setPos(glm::vec3 l);

    // Función para aplicar una matriz de transformación a la posición de la luz
    void aplicarTransformacion(const glm::mat4& matrizTransformacion);

    // Función para aplicar la luz al shader
    void aplicar(Shader& shader) const;
};

#endif
