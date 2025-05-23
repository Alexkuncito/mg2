#include "../include/entidad.hpp" 

// Definición de la clase MGEntity
MGEntity::MGEntity(Shader* shader) : shader(shader) {}

void MGEntity::draw([[maybe_unused]] glm::mat4 mat, [[maybe_unused]] std::optional<glm::vec4> color) {
    // Implementación vacía 2en la clase base
}

// Definición de la clase MGMesh
MGMesh::MGMesh(Shader* shader, Mesh* malla) : MGEntity(shader), malla(malla) {}

void MGMesh::draw(glm::mat4 mat, [[maybe_unused]] std::optional<glm::vec4> col ) {
    shader->setMat4("model", mat);

    
    malla->draw(shader, col);
}

// Definición de la clase MGCamara
MGCamara::MGCamara(Shader* shader, Camara* camara) : MGEntity(shader), camara(camara) {}

void MGCamara::draw([[maybe_unused]] glm::mat4 mat,  [[maybe_unused]] std::optional<glm::vec4> col ) {
    glm::mat4 view = camara->getViewMatrix();
    shader->setMat4("view", view);
}

// Definición de la clase MGLuz
MGLuz::MGLuz(Shader* shader, Luz* luz) : MGEntity(shader), luz(luz) {}

void MGLuz::draw([[maybe_unused]] glm::mat4 mat,  [[maybe_unused]] std::optional<glm::vec4> col ) {
    luz->aplicar(*shader);
}
