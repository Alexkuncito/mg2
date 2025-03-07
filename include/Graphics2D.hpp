#ifndef GRAPHICS2D_HPP
#define GRAPHICS2D_HPP

#include <map>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <freetype2/ft2build.h>
#include "Shader.hpp"
#include "Textura.hpp"
#include FT_FREETYPE_H

struct Character {
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};

class Graphics2D {
public:
    static glm::mat4 orthoProjection;
    static void init2D();
    static void DrawRectangle(float x, float y, float width, float height, glm::vec4 color);
    static void DrawCircle(float x, float y, float radius, glm::vec4 color);
    static void LoadFont(const std::string& fontPath, unsigned int fontSize);
    static void DrawText(std::string text, float x, float y, float scale, glm::vec3 color);
    static void DrawTexture(float x, float y, float width, float height, const Textura& texture);
private:
    static std::map<char, Character> Characters;
    static std::unique_ptr<Shader> shader2D;
    static std::unique_ptr<Shader> shaderText2D;
    static std::unique_ptr<Shader> shaderTexture2D;
    static unsigned int VAO, VBO;
};

#endif