#include "motortag.hpp"

std::shared_ptr<Nodo> TMotorTAG::crearNodo(std::shared_ptr<Nodo> padre, std::shared_ptr<MGEntity> ent, glm::vec3 traslacion, glm::vec3 escalado, glm::vec3 rotacion) {
    // Crear el nuevo nodo con la entidad y un ID único
    static int idCounter = 0;  // Usamos un contador estático para asignar un ID único
    std::shared_ptr<Nodo> nuevoNodo = std::make_shared<Nodo>(ent, idCounter++);

    // Establecer las transformaciones del nodo
    nuevoNodo->setTraslacion(traslacion);
    nuevoNodo->setEscalado(escalado);
    nuevoNodo->setRotacion(rotacion);

    // Si se pasa un nodo padre, agregar este nuevo nodo como hijo
    if (padre != nullptr) {
        padre->agregarHijo(nuevoNodo);
    }

    // Retornar el nuevo nodo creado
    return nuevoNodo;
}

std::shared_ptr<MGCamara> TMotorTAG::crearCamara(std::shared_ptr<Shader> shader, std::shared_ptr<Camara> camara){
    std::shared_ptr<MGCamara> nuevaCamara = std::make_shared<MGCamara>(shader, camara);
    return nuevaCamara;
}

std::shared_ptr<MGLuz> TMotorTAG::crearLuz(std::shared_ptr<Shader> shader, std::shared_ptr<Luz> luz){
    std::shared_ptr<MGLuz> nuevaLuz = std::make_shared<MGLuz>(shader, luz);
    return nuevaLuz;
}

int TMotorTAG::registrarCamara(std::shared_ptr<Nodo> cam){
    regCamaras.push_back(cam);
    return static_cast<int>(regCamaras.size()) - 1;
}

int TMotorTAG::registrarLuz(std::shared_ptr<Nodo> luz){
    regLuces.push_back(luz);
    return static_cast<int>(regLuces.size()) - 1;
}

void TMotorTAG::activarCamara(int v){
    if (v >= 0 && v < regCamaras.size()) {
        std::shared_ptr<MGCamara> camara = std::dynamic_pointer_cast<MGCamara>(regCamaras[v]->getEntidad());
        if (camara) {
            camara->activar();
        }
    }
}

void TMotorTAG::activarLuz(int v){
    if (v >= 0 && v < regLuces.size()) {
        std::shared_ptr<MGLuz> luz = std::dynamic_pointer_cast<MGLuz>(regLuces[v]->getEntidad());
        if (luz) {
            luz->activar();
        }
    }
}

void TMotorTAG::desActivarCamara(int v){
    if (v >= 0 && v < regCamaras.size()) {
        std::shared_ptr<MGCamara> camara = std::dynamic_pointer_cast<MGCamara>(regCamaras[v]->getEntidad());
        if (camara) {
            camara->desactivar();
        }
    }
}

void TMotorTAG::desActivarLuz(int v){
    if (v >= 0 && v < regLuces.size()) {
        std::shared_ptr<MGLuz> luz = std::dynamic_pointer_cast<MGLuz>(regLuces[v]->getEntidad());
        if (luz) {
            luz->desactivar();
        }
    }
}

void TMotorTAG::dibujarEscena() {
    for (auto& nodoCamara : regCamaras) {
        std::shared_ptr<MGCamara> camara = std::dynamic_pointer_cast<MGCamara>(nodoCamara->getEntidad());
        if (camara && camara->esActiva()) {
            camara.get()->draw(glm::mat4(1.0f));
            break;
        }
    }

    // Activar las luces
    int luzIndex = 0;
    for (auto& nodoLuz : regLuces) {
        // Solo activamos las luces que están activadas
        std::shared_ptr<MGLuz> luz = std::dynamic_pointer_cast<MGLuz>(nodoLuz->getEntidad());
        if (luz && luz->esActiva()) {
            luz.get()->draw(glm::mat4(1.0f));
        }
    }

    if (escena != nullptr) {
        escena->recorrer(glm::mat4(1.0f));
    }
}

