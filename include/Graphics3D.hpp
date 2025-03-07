#ifndef GRAPHICS3D_HPP
#define GRAPHICS3D_HPP

#include <map>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <freetype2/ft2build.h>
#include "Shader.hpp"
#include "Textura.hpp"
#include FT_FREETYPE_H

class Graphics3D {
public:
    static void DrawCube(float x, float y, float z, float width, float height, float depth, glm::vec4 color, Shader *shader);
private:
    static unsigned int VAO, VBO;
};

#endif