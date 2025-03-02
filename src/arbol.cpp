#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iomanip>
#include "entidad.cpp"

using namespace std;


void imprimirMatriz(const glm::mat4& mat);


// Definición de la estructura Nodo para representar un árbol jerárquico
struct Nodo {
    private:
        MGEntity* entidad; // En el futuro, este entidad representará una entidad en lugar de un entero
        int id;
        vector<Nodo*> hijos; // Lista de punteros a los nodos hijos
        Nodo* padre; // Puntero al nodo padre
        glm::vec3 traslacion; // Vector de traslación
        glm::vec3 rotacion; // Vector de rotación
        glm::vec3 escalado; // Vector de escalado
        glm::mat4 matrizTrasf; // Matriz de transformación acumulada
        bool actTrans; // Bandera para indicar si la transformación ha cambiado
    
    public:
        // Constructor que inicializa el nodo con valores por defecto
        explicit Nodo(MGEntity* valor, int id) : entidad(valor), id(id), padre(nullptr), traslacion(0.0f), rotacion(0.0f), escalado(1.0f), actTrans(false) {
            matrizTrasf = calcularMatriz(); // Calcula la matriz de transformación inicial
        }

        // Destructor para eliminar los nodos hijos y liberar memoria
        ~Nodo() {
            for (Nodo* hijo : hijos) {
                delete hijo;
            }
            hijos.clear();
        }

        // Método para calcular la matriz de transformación combinando traslación, rotación y escalado
        glm::mat4 calcularMatriz() {
            actTrans = false;
            return trasladar(traslacion) * rotar(rotacion) * escalar(escalado);
        }

        // Agrega un nodo hijo a la lista de hijos
        void agregarHijo(Nodo* nodo) {
            nodo->padre = this; // Establece este nodo como padre del nuevo hijo
            hijos.push_back(nodo);
            
        }

        // Borra un nodo hijo si existe en la lista
        void borrarHijo(Nodo* nodo) {
            for (auto it = hijos.begin(); it != hijos.end(); ++it) {
                if (*it == nodo) {
                    delete *it;
                    hijos.erase(it);
                }
            }// Retorna -1 si el nodo hijo no se encuentra
        }

        // Establece un nuevo valor de entidad en el nodo
        bool setEntidad(MGEntity* val) {
            entidad = val;
            return true;
        }

        // Obtiene un puntero al valor de la entidad
        MGEntity* getEntidad() {
            return entidad;
        }

        // Devuelve el nodo padre
        Nodo* getPadre() {
            return padre;
        }

        // Devuelve la lista de hijos
        vector<Nodo*> getHijos() {
            return hijos;
        }

        void activTrans(){
            actTrans = true;
            for (Nodo* hijo : hijos) {
                hijo->activTrans();
            }
        }

        // Recorre el árbol y aplica transformaciones acumuladas
        void recorrer(glm::mat4 matAcum) {
            if (actTrans) {
                matrizTrasf = matAcum * calcularMatriz();
            }
            
            //imprimirMatriz(matrizTrasf); //Llama a la función para imprimir la matriz de transformación
            if(entidad != NULL) {
                entidad->draw(matrizTrasf); // Llama a la función para dibujar el nodo
            }


            for (Nodo* hijo : hijos) {
                hijo->recorrer(matrizTrasf);
            }
        }

        // Métodos para modificar transformaciones y marcar cambios
        void setTraslacion(glm::vec3 vc) {
            traslacion = vc;
            activTrans();
        }

        void setRotacion(glm::vec3 vc) {
            rotacion = vc;
            activTrans();
        }
        
        void setEscalado(glm::vec3 vc) {
            escalado = vc;
            activTrans();
        }

        // Métodos para calcular transformaciones individuales
        glm::mat4 trasladar(glm::vec3 vc) {
            return glm::translate(glm::mat4(1.0f), vc);
        }

        glm::mat4 rotar(glm::vec3 vc) {
            glm::quat rotX = glm::angleAxis(glm::radians(vc.x), glm::vec3(1, 0, 0));
            glm::quat rotY = glm::angleAxis(glm::radians(vc.y), glm::vec3(0, 1, 0));
            glm::quat rotZ = glm::angleAxis(glm::radians(vc.z), glm::vec3(0, 0, 1));
            glm::quat rotFinal = rotZ * rotY * rotX;
            return glm::mat4_cast(rotFinal) * glm::mat4(1.0f);
        }

        glm::mat4 escalar(glm::vec3 vc) {
            return glm::scale(glm::mat4(1.0f), vc);
        }

        // Métodos para obtener valores de transformación
        glm::vec3 getTraslacion() { return traslacion; }
        glm::vec3 getRotacion() { return rotacion; }
        glm::vec3 getEscalado() { return escalado; }

        // Métodos para obtener y establecer la matriz de transformación
        void setMatrizTransf(glm::mat4 mat) {
            matrizTrasf = mat;
        }
        glm::mat4 getMatrizTransf() {
            return matrizTrasf;
        }
};

// Función para imprimir matrices en formato visual
void imprimirMatriz(const glm::mat4& mat) {
    cout << fixed << setprecision(6); // Formato de punto fijo con 6 decimales
    for (int i = 0; i < 4; ++i) {
        cout << "| ";
        for (int j = 0; j < 4; ++j) {
            cout << setw(12) << mat[i][j] << " | "; // Espaciado uniforme
        }
        cout << endl;
    }
    cout << endl;
}

// Función para imprimir la estructura del árbol
void imprimirArbol(Nodo* nodo, int nivel = 0) {
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
