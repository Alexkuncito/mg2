#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float speed;
    float sensitivity;
    float yaw;
    float pitch;

    Camera(glm::vec3 startPos, glm::vec3 startFront, glm::vec3 startUp, float startSpeed, float startSensitivity)
        : position(startPos), front(startFront), up(startUp), speed(startSpeed), sensitivity(startSensitivity), yaw(-90.0f), pitch(-45.0f) {updateCameraVectors();}

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    // Mueve la cámara hacia adelante/atrás en el plano horizontal
    void moveForward(float deltaTime) {
        glm::vec3 flatFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
        position += speed * flatFront * deltaTime;
    }

    void moveBackward(float deltaTime) {
        glm::vec3 flatFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
        position -= speed * flatFront * deltaTime;
    }

    void moveLeft(float deltaTime) {
        position -= glm::normalize(glm::cross(front, up)) * speed * deltaTime;
    }

    void moveRight(float deltaTime) {
        position += glm::normalize(glm::cross(front, up)) * speed * deltaTime;
    }

    // Zoom con flechas arriba/abajo (mueve la cámara en la dirección de la vista)
    void zoomIn(float deltaTime) {
        position += speed * front * deltaTime;
    }

    void zoomOut(float deltaTime) {
        position -= speed * front * deltaTime;
    }

    // Rotar la cámara con flechas izquierda/derecha
    void rotateLeft(float deltaTime) {
        yaw -= sensitivity * deltaTime;
        updateCameraVectors();
    }

    void rotateRight(float deltaTime) {
        yaw += sensitivity * deltaTime;
        updateCameraVectors();
    }

private:
    void updateCameraVectors() {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);
    }
};

#endif
