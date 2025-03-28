#include "entidad.hpp" 

// Definición de la clase MGEntity
MGEntity::MGEntity(Shader* shader) : shader(shader) {}

void MGEntity::draw(glm::mat4 mat) {
    // Implementación vacía en la clase base
}

// Definición de la clase MGMesh
MGMesh::MGMesh(Shader* shader, Mesh* malla) : MGEntity(shader), malla(malla) {}

void MGMesh::draw(glm::mat4 mat) {

    shader->setMat4("model", mat);
    malla->setMat(shader);
    malla->draw();
}

// Definición de la clase MGCamara
MGCamara::MGCamara(Shader* shader, Camara* camara) : MGEntity(shader), camara(camara) {}

void MGCamara::draw(glm::mat4 mat) {
    glm::mat4 view = camara->getViewMatrix();
    shader->setMat4("view", view);
}

// Definición de la clase MGLuz
MGLuz::MGLuz(Shader* shader, Luz* luz) : MGEntity(shader), luz(luz) {}

void MGLuz::draw(glm::mat4 mat) {
    luz->aplicar(*shader);
}
