#ifndef MOTORTAG_HPP
#define MOTORTAG_HPP

#include "gestorrecursos.hpp"
#include "arbol.hpp"
#include <vector>
#include <memory> // Para shared_ptr

struct TMotorTAG{
    private:
        TGestorRecursos gestorRecursos;

        std::unordered_map<int, std::unique_ptr<Nodo>> gnodos;

        Nodo* escena;
        int cantnode;

        std::vector<Nodo*> regCamaras;
        std::vector<Nodo*> regLuces;

    public:
        TMotorTAG(Shader* shader) : cantnode(0), escena(nullptr){     //Cambiar para que tome el shader ese
            gnodos[cantnode] = make_unique<Nodo>();
            escena = gnodos[cantnode].get();
            cantnode+=1;
        }


        // Cambiar los parámetros para que usen shared_ptr
        Nodo* crearNodo(Nodo* padre, MGEntity* ent, glm::vec3 traslacion, glm::vec3 escalado, glm::vec3  rotacion);
        MGCamara crearCamara(Shader* shader, Camara* camara);
        MGLuz crearLuz(Shader* shader, Luz* luz);
        MGMesh crearMalla(Shader* shader, const Fichero& fichero, 
        std::optional<std::reference_wrapper<const Textura>> textura = std::nullopt, 
        std::optional<TMaterial> material = std::nullopt);

        void deleteCamara(MGCamara* cam);
        void deleteLuz(MGLuz* luz);
        void deleteMalla(MGMesh* mall);

        int registrarCamara(Nodo* cam);
        int registrarLuz(Nodo* luz);

        void activarCamara(int v);
        void activarLuz(int v);

        void desActivarCamara(int v);
        void desActivarLuz(int v);

        void dibujarEscena();

        void pinta(){
            gestorRecursos.ImprimirRecursos();
            if (escena) {
                imprimirArbol(escena, 0);
            } else {
                std::cout << "Error: escena es nullptr" << std::endl;
            }
        }


        Nodo* getRaiz(){return escena;};

    Camara* getCamaraActiva() {
        if (!regCamaras.empty()) {
            Nodo* nodoCamara = regCamaras[0];
            MGEntity* entidad = nodoCamara->getEntidad();
            
            MGCamara* mgCamara = dynamic_cast<MGCamara*>(entidad);
            
            if (mgCamara) {
                return mgCamara->getCamera();
            } else {
                std::cout << "No se pudo convertir la entidad a MGCamara" << std::endl;
                return nullptr;
            }
        } else {
            std::cout << "No detecta cámaras" << std::endl;
            return nullptr;
        }
    }

};

#endif
