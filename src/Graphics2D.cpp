#include "Graphics2D.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <memory>

std::unique_ptr<Shader> Graphics2D::shader2D = nullptr;
std::unique_ptr<Shader> Graphics2D::shaderText2D = nullptr;
std::unique_ptr<Shader> Graphics2D::shaderTexture2D = nullptr;

glm::mat4 Graphics2D::orthoProjection;

void Graphics2D::init2D() {
    shader2D = std::make_unique<Shader>("../shaders/vertex_2d.glsl", "../shaders/fragment_2d.glsl");
    shaderText2D = std::make_unique<Shader>("../shaders/vertex_text2d.glsl", "../shaders/fragment_text2d.glsl");
    shaderTexture2D = std::make_unique<Shader>("../shaders/vertex_texture2d.glsl", "../shaders/fragment_texture2d.glsl");
    orthoProjection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
}

void Graphics2D::DrawRectangle(float x, float y, float width, float height, glm::vec4 color) {
    shader2D->use();
    shader2D->setMat4("projection", orthoProjection);
    shader2D->setVec4("color", color);

    glDisable(GL_DEPTH_TEST);

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

    glDisable(GL_DEPTH_TEST);

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
        FT_Done_FreeType(ft);
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "Error: No se pudo cargar la fuente " << fontPath << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Error: No se pudo cargar el carácter " << c << std::endl;
            continue;
        }
        bool tiene_bitmap = (face->glyph->bitmap.buffer != nullptr && face->glyph->bitmap.width > 0);
        unsigned int texture= 0;

        if (tiene_bitmap) {
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
            int width, height;
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
            std::cout << "Textura generada: [" << c << "] - Tamaño: " << width << "x" << height << std::endl;
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        if (c == ' '  && face->glyph->advance.x == 0) {
            face->glyph->advance.x = 10 << 6;

        }
        
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

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Graphics2D::DrawText(std::string text, float x, float y, float scale, glm::vec3 color) {
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GLenum error;
    error = glGetError();
    std::cout << "Error antes de usar shaderText2D: " << error << std::endl;

    shaderText2D->use();
    shaderText2D->setInt("text", 0);
    shaderText2D->setMat4("projection", orthoProjection);
    shaderText2D->setVec3("textColor", color);

    error = glGetError();
    std::cout << "Error después de usar shaderText2D: " << error << std::endl;
    
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
        std::cout << "Dibujando carácter: " << c 
          << " - x: " << xpos << ", y: " << ypos
          << ", w: " << w << ", h: " << h << std::endl;

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        GLint tex;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex);
        std::cout << "Textura activa para " << c << ": " << tex << std::endl;

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * scale; 
    }
    std::cout << std::endl;
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics2D::DrawTexture(float x, float y, float width, float height, const Textura& texture) {
    shaderTexture2D->use();
    shaderTexture2D->setMat4("projection", orthoProjection);

    glDisable(GL_DEPTH_TEST);
    
    float vertices[] = {
        x, y, 0.0f, 0.0f, 1.0f, // Top-left
        x + width, y, 0.0f, 1.0f, 1.0f, // Top-right
        x + width, y - height, 0.0f, 1.0f, 0.0f, // Bottom-right
        x, y - height, 0.0f, 0.0f, 0.0f  // Bottom-left
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    texture.bind();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    texture.unbind();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
