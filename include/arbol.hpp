#ifndef ARBOL_HPP
#define ARBOL_HPP

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "entidad.hpp"

using namespace std;

class Nodo{
    private:
        MGEntity* entidad;
        vector<Nodo*> hijos;
        Nodo* padre; // weak_ptr para evitar ciclos de referencia
        glm::vec3 traslacion=glm::vec3(0.0f);
        glm::vec3 rotacion=glm::vec3(0.0f);
        glm::vec3 escalado=glm::vec3(0.005f);
        glm::mat4 matrizTrasf;
        bool actTrans;
    
    public:
        explicit Nodo(MGEntity* valor);
        Nodo() : entidad(nullptr), padre(nullptr), matrizTrasf(1.0f), actTrans(false) {} // Constructor por defecto

        ~Nodo() = default; // No se necesita destructor explícito con smart pointers

        glm::mat4 calcularMatriz();
        void agregarHijo(Nodo* nodo);
        void borrarHijo(Nodo* nodo);
        bool setEntidad(MGEntity* val);
        MGEntity* getEntidad();
        Nodo* getPadre();
        vector<Nodo*> getHijos();
        void activTrans();
        void recorrer(glm::mat4 matAcum);
        void setTraslacion(glm::vec3 vc);
        void setRotacion(glm::vec3 vc);
        void setEscalado(glm::vec3 vc);
        glm::mat4 trasladar(glm::vec3 vc);
        glm::mat4 rotar(glm::vec3 vc);
        glm::mat4 escalar(glm::vec3 vc);
        glm::vec3 getTraslacion();
        glm::vec3 getRotacion();
        glm::vec3 getEscalado();
        void setMatrizTransf(glm::mat4 mat);
        glm::mat4 getMatrizTransf();
};

// Funciones globales
void imprimirMatriz(const glm::mat4& mat);
void imprimirArbol(Nodo* nodo, int nivel = 0);

#endif
