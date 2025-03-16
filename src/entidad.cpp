#include "entidad.hpp" 

// Definición de la clase MGEntity
MGEntity::MGEntity(std::shared_ptr<Shader> shader) : shader(shader) {}

void MGEntity::draw(glm::mat4 mat) {
    // Implementación vacía en la clase base
    std::cout << "Aqui no deberia entrar" << std::endl;
}

// Definición de la clase MGMesh
MGMesh::MGMesh(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> malla) : MGEntity(shader), malla(malla) {}

void MGMesh::draw(glm::mat4 mat) {
    std::cout << "DIBUJANDO... MALLA..." << std::endl;

    shader->setMat4("model", mat);
    malla->setMat(shader.get());
    malla->draw();
}

// Definición de la clase MGCamara
MGCamara::MGCamara(std::shared_ptr<Shader> shader, std::shared_ptr<Camara> camara) : MGEntity(shader), camara(camara) {}

void MGCamara::draw(glm::mat4 mat) {
    std::cout << "DIBUJANDO... CAMARA..." << std::endl;
    glm::mat4 view = camara->getViewMatrix();
    shader->setMat4("view", view);
}

// Definición de la clase MGLuz
MGLuz::MGLuz(std::shared_ptr<Shader> shader, std::shared_ptr<Luz> luz) : MGEntity(shader), luz(luz) {}

void MGLuz::draw(glm::mat4 mat) {
    std::cout << "DIBUJANDO... LUZ..." << std::endl;
    luz->aplicar(*shader);
}
