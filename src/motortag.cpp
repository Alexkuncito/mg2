#include "motortag.hpp"

Nodo* TMotorTAG::crearNodo(Nodo* padre, MGEntity* ent, glm::vec3 traslacion, glm::vec3 escalado, glm::vec3 rotacion) {
    // Crear el nuevo nodo con la entidad y un ID único
    static int idCounter = 0;  // Usamos un contador estático para asignar un ID único
    Nodo* nuevoNodo = new Nodo(ent, idCounter++);

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

MGCamara* TMotorTAG::crearCamara(Shader* shader, Camara* camara){
    MGCamara* nuevaCamara = new MGCamara(shader, camara);
    return nuevaCamara;
}

MGLuz* TMotorTAG::crearLuz(Shader* shader, Luz* luz){
    MGLuz* nuevaLuz = new MGLuz(shader, luz);
    return nuevaLuz;
}


void TMotorTAG::deleteCamara(MGCamara* cam) {
    if (cam != nullptr) {
        delete cam;
    }
}

void TMotorTAG::deleteLuz(MGLuz* luz) {
    if (luz != nullptr) {
        delete luz;
    }
}

void TMotorTAG::deleteMalla(MGMesh* malla) {
    if (malla != nullptr) {
        delete malla;
    }
}

int TMotorTAG::registrarCamara(Nodo* cam){
    regCamaras.push_back(cam);
    return static_cast<int>(regCamaras.size()) -1;
}

int TMotorTAG::registrarLuz(Nodo* luz){
    regLuces.push_back(luz);
    return static_cast<int>(regLuces.size()) -1;
}

void TMotorTAG::activarCamara(int v){
    if (v >= 0 && v < regCamaras.size()) {
        MGCamara* camara = dynamic_cast<MGCamara*>(regCamaras[v]->getEntidad());
        if (camara) {
            camara->activar();
        }
    }
}
void TMotorTAG::activarLuz(int v){
    if (v >= 0 && v < regLuces.size()) {
        MGLuz* luz = dynamic_cast<MGLuz*>(regLuces[v]->getEntidad());
        if (luz) {
            luz->activar();
        }
    }
}

void TMotorTAG::desActivarCamara(int v){
    if (v >= 0 && v < regCamaras.size()) {
        MGCamara* camara = dynamic_cast<MGCamara*>(regCamaras[v]->getEntidad());
        if (camara) {
            camara->desactivar();
        }
    }
}

void TMotorTAG::desActivarLuz(int v){
    if (v >= 0 && v < regLuces.size()) {
        MGLuz* luz = dynamic_cast<MGLuz*>(regLuces[v]->getEntidad());
        if (luz) {
            luz->desactivar();
        }
    }
}