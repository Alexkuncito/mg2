#ifndef CAMARA_HPP
#define CAMARA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camara {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float speed;
    float sensitivity;
    float yaw;
    float pitch;

    Camara(glm::vec3 startPos, glm::vec3 startFront, glm::vec3 startUp, float startSpeed, float startSensitivity);

    glm::mat4 getViewMatrix() const;

    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void zoomIn(float deltaTime);
    void zoomOut(float deltaTime);
    void rotateLeft(float deltaTime);
    void rotateRight(float deltaTime);

private:
    void updateCamaraVectors();
};

#endif
