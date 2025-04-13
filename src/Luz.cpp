#include "Luz.hpp"

// Constructor
Luz::Luz(const glm::vec3& pos, const glm::vec3& col, float inten)
    : posicion(pos), color(col), intensidad(inten) {}

// Método para obtener la posición de la luz
glm::vec3 Luz::getPos() {
    return posicion;
}

// Método para establecer la posición de la luz
void Luz::setPos(glm::vec3 l) {
    posicion = l;
}

// Método para aplicar la transformación a la posición de la luz
void Luz::aplicarTransformacion(const glm::mat4& matrizTransformacion) {
    glm::vec4 posHomogenea = glm::vec4(posicion, 1.0f);  // Convertir a coordenadas homogéneas
    posHomogenea = matrizTransformacion * posHomogenea;  // Multiplicar por la matriz de transformación

    // Convertir la posición de nuevo a 3D
    posicion = glm::vec3(posHomogenea); // Convertir de vuelta a coordenadas 3D
}

// Método para aplicar la luz al shader
void Luz::aplicar(Shader& shader) const {
    shader.use();
    shader.setVec3("luzPos", posicion);
    shader.setVec3("luzColor", color * intensidad);
}
