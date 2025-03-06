#include "Graphics2D.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <memory>

std::unique_ptr<Shader> Graphics2D::shader2D = nullptr;
std::unique_ptr<Shader> Graphics2D::shaderText2D = nullptr;

glm::mat4 Graphics2D::orthoProjection;

void Graphics2D::init2D() {
    shader2D = std::make_unique<Shader>("../shaders/vertex_2d.glsl", "../shaders/fragment_2d.glsl");
    shaderText2D = std::make_unique<Shader>("../shaders/vertex_text2d.glsl", "../shaders/fragment_text2d.glsl");
    orthoProjection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
}

void Graphics2D::DrawRectangle(float x, float y, float width, float height, glm::vec4 color) {
    shader2D->use();
    shader2D->setMat4("projection", orthoProjection);
    shader2D->setVec4("color", color);

    float vertices[] = {
        x, y, 0.0f,         // Top-left
        x + width, y, 0.0f, // Top-right
        x + width, y - height, 0.0f, // Bottom-right
        x, y - height, 0.0f // Bottom-left
    };

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Graphics2D::DrawCircle(float x, float y, float radius, glm::vec4 color) {
    shader2D->use();
    shader2D->setMat4("projection", orthoProjection);
    shader2D->setVec4("color", color);
    
    const int numSegments = 50;
    float vertices[numSegments * 3];

    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        vertices[i * 3] = x + dx;
        vertices[i * 3 + 1] = y + dy;
        vertices[i * 3 + 2] = 0.0f;
    }

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

std::map<char, Character> Graphics2D::Characters;
unsigned int Graphics2D::VAO, Graphics2D::VBO;

void Graphics2D::LoadFont(const std::string& fontPath, unsigned int fontSize) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Error: No se pudo inicializar FreeType" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "Error: No se pudo cargar la fuente " << fontPath << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Deshabilita el alineamiento de byte

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Error: No se pudo cargar el carácter " << c << std::endl;
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Graphics2D::RenderText(std::string text, float x, float y, float scale, glm::vec3 color) {
    shaderText2D->use();
    shaderText2D->setMat4("projection", orthoProjection);
    shaderText2D->setVec3("textColor", color);
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (char c : text) {
        Character ch = Characters[c];

        std::cout << c;

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },   
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        if (ch.TextureID == 0) {
            std::cerr << "Error: No se pudo cargar la textura para el carácter: " << c << std::endl;
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * scale; 
    }
    std::cout << std::endl;
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}