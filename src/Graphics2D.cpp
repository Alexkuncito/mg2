#include "Graphics2D.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <memory>

std::unique_ptr<Shader> Graphics2D::shader2D = nullptr;

void Graphics2D::init2D() {
    shader2D = std::make_unique<Shader>("../shaders/vertex_2d.glsl", "../shaders/fragment_2d.glsl");
}

void Graphics2D::DrawRectangle(float x, float y, float width, float height, glm::vec4 color) {
    shader2D->use();
    glm::mat4 orthoProjection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
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
    glm::mat4 orthoProjection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
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