#ifndef MOTORTAG_HPP
#define MOTORTAG_HPP

#include "gestorrecursos.hpp"
#include "arbol.hpp"
#include <vector>


struct TMotorTAG{
    private:
        Nodo* escena;
        TGestorRecursos* gestorRecursos;
        
        std::vector<Nodo*> regCamaras;
        std::vector<Nodo*> regLuces;
    public:
        Nodo* crearNodo(Nodo* padre,MGEntity* ent, glm::vec3 traslacion, glm::vec3 escalado, glm::vec3  rotacion);
        MGCamara* crearCamara(Shader* shader, Camara* camara);
        MGLuz* crearLuz(Shader* shader, Luz* luz);
        MGMesh* crearMalla(Shader* shader, const Fichero& fichero, 
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
};

#endif