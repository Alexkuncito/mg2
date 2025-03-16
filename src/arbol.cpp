#include "arbol.hpp"
#include <iostream>
#include <iomanip>

// Implementación de la clase Nodo
Nodo::Nodo(std::shared_ptr<MGEntity> valor, int id) 
    : entidad(valor), id(id), traslacion(0.0f), rotacion(0.0f), escalado(1.0f), actTrans(true) {
    matrizTrasf = calcularMatriz();
}

glm::mat4 Nodo::calcularMatriz() {
    actTrans = false;
    return trasladar(traslacion) * rotar(rotacion) * escalar(escalado);
}

void Nodo::agregarHijo(std::shared_ptr<Nodo> nodo) {
    nodo->padre = shared_from_this();
    hijos.push_back(nodo);
}


void Nodo::borrarHijo(std::shared_ptr<Nodo> nodo) {
    hijos.erase(std::remove(hijos.begin(), hijos.end(), nodo), hijos.end());
}

bool Nodo::setEntidad(std::shared_ptr<MGEntity> val) {
    entidad = val;
    return true;
}

std::shared_ptr<MGEntity> Nodo::getEntidad() {
    return entidad;
}

std::shared_ptr<Nodo> Nodo::getPadre() {
    return padre.lock(); // Convierte weak_ptr a shared_ptr
}

std::vector<std::shared_ptr<Nodo>> Nodo::getHijos() {
    return hijos;
}

void Nodo::activTrans() {
    actTrans = true;
    for (auto& hijo : hijos) {
        hijo->activTrans();
    }
}

void Nodo::recorrer(glm::mat4 matAcum) {
    std::cout << "ENTRA JODER" << std::endl;

    if (actTrans) {
        matrizTrasf = matAcum * calcularMatriz();
    }
    
    if (entidad) {
        entidad->draw(matrizTrasf);
    }
    else {
        std::cout << "no se ve coño" << std::endl;
    }
    
    for (auto& hijo : hijos) {
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
    std::cout << std::fixed << std::setprecision(6);
    for (int i = 0; i < 4; ++i) {
        std::cout << "| ";
        for (int j = 0; j < 4; ++j) {
            std::cout << std::setw(12) << mat[i][j] << " | ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void imprimirArbol(std::shared_ptr<Nodo> nodo, int nivel) {
    for (int i = 0; i < nivel; ++i) {
        std::cout << "   ";
    }
    if (nivel > 0) {
        std::cout << "+ ";
    }
    std::cout << "Nodo " << nodo->getId() << "." << std::endl;

    for (auto& hijo : nodo->getHijos()) {
        imprimirArbol(hijo, nivel + 1);
    }
}

