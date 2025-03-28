#include "motortag.hpp"

Nodo* TMotorTAG::crearNodo(Nodo* padre, MGEntity* ent, glm::vec3 traslacion, glm::vec3 escalado, glm::vec3 rotacion) {
    gnodos[cantnode] = make_unique<Nodo>(ent);
    Nodo* nuevoNodo = gnodos[cantnode].get();
    cantnode += 1;

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

MGCamara TMotorTAG::crearCamara(Shader* shader, Camara* camara){
    MGCamara nuevaCamara(shader, camara);
    return nuevaCamara;
}

MGLuz TMotorTAG::crearLuz(Shader* shader, Luz* luz){
    MGLuz nuevaLuz(shader, luz);
    return nuevaLuz;
}

int TMotorTAG::registrarCamara(Nodo* cam){
    regCamaras.push_back(cam);
    return static_cast<int>(regCamaras.size()) - 1;
}

int TMotorTAG::registrarLuz(Nodo* luz){
    regLuces.push_back(luz);
    return static_cast<int>(regLuces.size()) - 1;
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
        MGCamara* camara = dynamic_cast<MGCamara*>(regCamaras[v]->getEntidad());        if (camara) {
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

void TMotorTAG::dibujarEscena() {
    for (auto& nodoCamara : regCamaras) {
        MGCamara* camara = dynamic_cast<MGCamara*>(nodoCamara->getEntidad());
        if (camara && camara->esActiva()) {
            camara->draw(glm::mat4(1.0f));
            break;
        }
    }

    // Activar las luces
    int luzIndex = 0;
    for (auto& nodoLuz : regLuces) {
        // Solo activamos las luces que están activadas
        MGLuz* luz = dynamic_cast<MGLuz*>(nodoLuz->getEntidad());
        if (luz && luz->esActiva()) {
            luz->draw(glm::mat4(1.0f));
        }
    }

    if (escena != nullptr) {
        escena->recorrer(glm::mat4(1.0f));
    }
}


MGMesh TMotorTAG::crearMalla(Shader* shader, const Fichero& fichero, 
                                              std::optional<std::reference_wrapper<const Textura>> textura, 
                                              std::optional<TMaterial> material) {
    
    Recurso* recurso = gestorRecursos.getRecurso(fichero.getRuta());
    RecursoMalla* recursoMalla = dynamic_cast<RecursoMalla*>(recurso);

    if (!recursoMalla) {
        std::unique_ptr<RecursoMalla> rcm = std::make_unique<RecursoMalla>(fichero, textura, material);
        recursoMalla = rcm.get();
        gestorRecursos.add(std::move(rcm));
    }

    Mesh* mallaMesh = recursoMalla->returnMalla();

    MGMesh malla(shader, mallaMesh);

    return malla;
}



