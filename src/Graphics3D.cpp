#include "Graphics3D.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <memory>

void Graphics3D::DrawCube(float x, float y, float z, float width, float height, float depth, glm::vec4 color, Shader *shader) {
    // Vértices del cubo (cubriendo las 8 esquinas)
    float vertices[] = {
        // Posiciones              // Colores
        x - width / 2, y - height / 2, z - depth / 2,  color.r, color.g, color.b, color.a,
        x + width / 2, y - height / 2, z - depth / 2,  color.r, color.g, color.b, color.a,
        x + width / 2, y + height / 2, z - depth / 2,  color.r, color.g, color.b, color.a,
        x - width / 2, y + height / 2, z - depth / 2,  color.r, color.g, color.b, color.a,
        x - width / 2, y - height / 2, z + depth / 2,  color.r, color.g, color.b, color.a,
        x + width / 2, y - height / 2, z + depth / 2,  color.r, color.g, color.b, color.a,
        x + width / 2, y + height / 2, z + depth / 2,  color.r, color.g, color.b, color.a,
        x - width / 2, y + height / 2, z + depth / 2,  color.r, color.g, color.b, color.a
    };

    // Índices para los triángulos (dos para cada cara del cubo)
    unsigned int indices[] = {
        // Cara frontal
        0, 1, 2, 0, 2, 3,
        // Cara trasera
        4, 5, 6, 4, 6, 7,
        // Cara izquierda
        0, 3, 7, 0, 7, 4,
        // Cara derecha
        1, 2, 6, 1, 6, 5,
        // Cara superior
        2, 3, 7, 2, 7, 6,
        // Cara inferior
        0, 1, 5, 0, 5, 4
    };

    // Crear VAO y VBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Posición del vértice
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Usar el shader
    shader->use();
    shader->setVec4("color", color);

    // Dibujar el cubo
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // Limpiar
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    shader->setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}
