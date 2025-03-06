#ifndef GRAPHICS2D_HPP
#define GRAPHICS2D_HPP

#include <glm/glm.hpp>
#include <memory>
#include "Shader.hpp"

class Graphics2D {
public:
    //static void init2D();
    static void DrawRectangle(float x, float y, float width, float height, glm::vec4 color,Shader* shader2D);
    static void DrawCircle(float x, float y, float radius, glm::vec4 color, Shader* shader2D);
};

#endif // GRAPHICS2D_HPP
