#ifndef ARBOL_HPP
#define ARBOL_HPP

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "entidad.hpp"

using namespace std;

class Nodo : public std::enable_shared_from_this<Nodo>{
    private:
        shared_ptr<MGEntity> entidad;
        int id;
        vector<shared_ptr<Nodo>> hijos;
        weak_ptr<Nodo> padre; // weak_ptr para evitar ciclos de referencia
        glm::vec3 traslacion=glm::vec3(0.0f);
        glm::vec3 rotacion=glm::vec3(0.0f);
        glm::vec3 escalado=glm::vec3(0.005f);
        glm::mat4 matrizTrasf;
        bool actTrans;
    
    public:
        explicit Nodo(shared_ptr<MGEntity> valor, int id);
        ~Nodo() = default; // No se necesita destructor explícito con smart pointers

        glm::mat4 calcularMatriz();
        void agregarHijo(shared_ptr<Nodo> nodo);
        void borrarHijo(shared_ptr<Nodo> nodo);
        bool setEntidad(shared_ptr<MGEntity> val);
        shared_ptr<MGEntity> getEntidad();
        shared_ptr<Nodo> getPadre();
        vector<shared_ptr<Nodo>> getHijos();
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
        int getId() const {
            return id;
        }

};

// Funciones globales
void imprimirMatriz(const glm::mat4& mat);
void imprimirArbol(shared_ptr<Nodo> nodo, int nivel = 0);

#endif
