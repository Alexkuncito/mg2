#include "Camara.hpp"

Camara::Camara(glm::vec3 startPos, glm::vec3 startFront, glm::vec3 startUp, float startSpeed, float startSensitivity)
    : position(startPos), front(startFront), up(startUp), speed(startSpeed), sensitivity(startSensitivity), yaw(-90.0f), pitch(-45.0f) {
    updateCamaraVectors();
}

glm::mat4 Camara::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camara::moveForward(float deltaTime) {
    glm::vec3 flatFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    position += speed * flatFront * deltaTime;
}

void Camara::moveBackward(float deltaTime) {
    glm::vec3 flatFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    position -= speed * flatFront * deltaTime;
}

void Camara::moveLeft(float deltaTime) {
    position -= glm::normalize(glm::cross(front, up)) * speed * deltaTime;
}

void Camara::moveRight(float deltaTime) {
    position += glm::normalize(glm::cross(front, up)) * speed * deltaTime;
}

void Camara::zoomIn(float deltaTime) {
    position += speed * front * deltaTime;
}

void Camara::zoomOut(float deltaTime) {
    position -= speed * front * deltaTime;
}

void Camara::rotateLeft(float deltaTime) {
    yaw -= sensitivity * deltaTime;
    updateCamaraVectors();
}

void Camara::rotateRight(float deltaTime) {
    yaw += sensitivity * deltaTime;
    updateCamaraVectors();
}

void Camara::updateCamaraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
}
