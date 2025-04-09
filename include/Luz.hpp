#ifndef LUZ_HPP
#define LUZ_HPP

#include <glm/glm.hpp>
#include "Shader.hpp"

class Luz {
public:
    glm::vec3 posicion;
    glm::vec3 color;
    float intensidad;

    Luz(const glm::vec3& pos, const glm::vec3& col, float inten)
        : posicion(pos), color(col), intensidad(inten) {}

    glm::vec3 getPos() { return posicion; }
    void setPos(glm::vec3 l) { posicion = l; }

    // Función para aplicar una matriz de transformación a la posición de la luz
    void aplicarTransformacion(const glm::mat4& matrizTransformacion) {
        // Aplicar la matriz de transformación a la posición de la luz
        glm::vec4 posHomogenea = glm::vec4(posicion, 1.0f); // Convertir a coordenadas homogéneas
        posHomogenea = matrizTransformacion * posHomogenea;  // Multiplicar por la matriz de transformación

        // Convertir la posición de nuevo a 3D
        posicion = glm::vec3(posHomogenea); // Convertir de vuelta a coordenadas 3D
    }

    // Función para aplicar la luz al shader
    void aplicar(Shader& shader) const {
        shader.use();
        shader.setVec3("luzPos", posicion);
        shader.setVec3("luzColor", color * intensidad);
    }
};

#endif
