#include "arbol.hpp"
#include <iostream>
#include <iomanip>

// Implementación de la clase Nodo
Nodo::Nodo(MGEntity* valor, int id) : entidad(valor), id(id), padre(nullptr), traslacion(0.0f), rotacion(0.0f), escalado(1.0f), actTrans(false) {
    matrizTrasf = calcularMatriz();
}

Nodo::~Nodo() {
    for (Nodo* hijo : hijos) {
        delete hijo;
    }
    hijos.clear();
}

glm::mat4 Nodo::calcularMatriz() {
    actTrans = false;
    return trasladar(traslacion) * rotar(rotacion) * escalar(escalado);
}

void Nodo::agregarHijo(Nodo* nodo) {
    nodo->padre = this;
    hijos.push_back(nodo);
}

void Nodo::borrarHijo(Nodo* nodo) {
    for (auto it = hijos.begin(); it != hijos.end(); ++it) {
        if (*it == nodo) {
            delete *it;
            hijos.erase(it);
        }
    }
}

bool Nodo::setEntidad(MGEntity* val) {
    entidad = val;
    return true;
}

MGEntity* Nodo::getEntidad() {
    return entidad;
}

Nodo* Nodo::getPadre() {
    return padre;
}

vector<Nodo*> Nodo::getHijos() {
    return hijos;
}

void Nodo::activTrans() {
    actTrans = true;
    for (Nodo* hijo : hijos) {
        hijo->activTrans();
    }
}

void Nodo::recorrer(glm::mat4 matAcum) {
    if (actTrans) {
        matrizTrasf = matAcum * calcularMatriz();
    }

    if(entidad != NULL) {
        entidad->draw(matrizTrasf); // Llama a la función para dibujar el nodo
    }

    for (Nodo* hijo : hijos) {
        hijo->recorrer(matrizTrasf);
    }
}

void Nodo::setTraslacion(glm::vec3 vc) {
    traslacion = vc;
    activTrans();
}

void Nodo::setRotacion(glm::vec3 vc) {
    rotacion = vc;
    activTrans();
}

void Nodo::setEscalado(glm::vec3 vc) {
    escalado = vc;
    activTrans();
}

glm::mat4 Nodo::trasladar(glm::vec3 vc) {
    return glm::translate(glm::mat4(1.0f), vc);
}

glm::mat4 Nodo::rotar(glm::vec3 vc) {
    glm::quat rotX = glm::angleAxis(glm::radians(vc.x), glm::vec3(1, 0, 0));
    glm::quat rotY = glm::angleAxis(glm::radians(vc.y), glm::vec3(0, 1, 0));
    glm::quat rotZ = glm::angleAxis(glm::radians(vc.z), glm::vec3(0, 0, 1));
    glm::quat rotFinal = rotZ * rotY * rotX;
    return glm::mat4_cast(rotFinal) * glm::mat4(1.0f);
}

glm::mat4 Nodo::escalar(glm::vec3 vc) {
    return glm::scale(glm::mat4(1.0f), vc);
}

glm::vec3 Nodo::getTraslacion() { return traslacion; }
glm::vec3 Nodo::getRotacion() { return rotacion; }
glm::vec3 Nodo::getEscalado() { return escalado; }

void Nodo::setMatrizTransf(glm::mat4 mat) {
    matrizTrasf = mat;
}

glm::mat4 Nodo::getMatrizTransf() {
    return matrizTrasf;
}

// Funciones globales
void imprimirMatriz(const glm::mat4& mat) {
    cout << fixed << setprecision(6);
    for (int i = 0; i < 4; ++i) {
        cout << "| ";
        for (int j = 0; j < 4; ++j) {
            cout << setw(12) << mat[i][j] << " | ";
        }
        cout << endl;
    }
    cout << endl;
}

void imprimirArbol(Nodo* nodo, int nivel) {
    for (int i = 0; i < nivel; ++i) {
        cout << "   ";
    }
    if (nivel > 0) {
        cout << "+ ";
    }
    cout << "Nodo " << "UNO" << "." << endl;

    for (Nodo* hijo : nodo->getHijos()) {
        imprimirArbol(hijo, nivel + 1);
    }
}
