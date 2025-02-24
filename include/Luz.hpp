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

    void aplicar(Shader& shader) const {
        shader.use();
        shader.setVec3("luzPos", posicion);
        shader.setVec3("luzColor", color * intensidad);
    }
};

#endif
