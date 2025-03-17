#ifndef MOTORTAG_HPP
#define MOTORTAG_HPP

#include "gestorrecursos.hpp"
#include "arbol.hpp"
#include <vector>
#include <memory> // Para shared_ptr

struct TMotorTAG{
    private:
        std::shared_ptr<Nodo> escena;  // Usar shared_ptr en lugar de Nodo*
        std::shared_ptr<TGestorRecursos> gestorRecursos;  // Usar shared_ptr en lugar de TGestorRecursos*

        std::vector<std::shared_ptr<Nodo>> regCamaras;  // Usar shared_ptr en lugar de Nodo*
        std::vector<std::shared_ptr<Nodo>> regLuces;    // Usar shared_ptr en lugar de Nodo*

    public:
        TMotorTAG(std::shared_ptr<Shader> shader) {
            
            std::shared_ptr<MGEntity> entVACIA = std::make_shared<MGEntity>(shader);

            // Crear el nodo raíz
            escena = std::make_shared<Nodo>(entVACIA, 0);

            // Inicializamos el gestor de recursos
            gestorRecursos = std::make_shared<TGestorRecursos>();

            // Inicializamos las cámaras y luces
            regCamaras = std::vector<std::shared_ptr<Nodo>>();
            regLuces = std::vector<std::shared_ptr<Nodo>>();
        }


        // Cambiar los parámetros para que usen shared_ptr
        std::shared_ptr<Nodo> crearNodo(std::shared_ptr<Nodo> padre, std::shared_ptr<MGEntity> ent, glm::vec3 traslacion, glm::vec3 escalado, glm::vec3  rotacion);
        std::shared_ptr<MGCamara> crearCamara(std::shared_ptr<Shader> shader, std::shared_ptr<Camara> camara);
        std::shared_ptr<MGLuz> crearLuz(std::shared_ptr<Shader> shader, std::shared_ptr<Luz> luz);
        std::shared_ptr<MGMesh> crearMalla(std::shared_ptr<Shader> shader, const Fichero& fichero, 
        std::optional<std::reference_wrapper<const Textura>> textura = std::nullopt, 
        std::optional<TMaterial> material = std::nullopt);

        void deleteCamara(std::shared_ptr<MGCamara> cam);
        void deleteLuz(std::shared_ptr<MGLuz> luz);
        void deleteMalla(std::shared_ptr<MGMesh> mall);

        int registrarCamara(std::shared_ptr<Nodo> cam);
        int registrarLuz(std::shared_ptr<Nodo> luz);

        void activarCamara(int v);
        void activarLuz(int v);

        void desActivarCamara(int v);
        void desActivarLuz(int v);

        void dibujarEscena();

        void pinta(){gestorRecursos.get()->ImprimirRecursos();}


        std::shared_ptr<Nodo> getRaiz(){return escena;};

    std::shared_ptr<Camara> getCamaraActiva() {
    if (!regCamaras.empty()) {
        std::shared_ptr<Nodo> nodoCamara = regCamaras[0];
        std::shared_ptr<MGEntity> entidad = nodoCamara->getEntidad();
        
        // Imprime el tipo de entidad para depuración
        std::cout << "Tipo de entidad: " << typeid(*entidad).name() << std::endl;
        
        // Intenta hacer la conversión de la entidad a MGCamara
        std::shared_ptr<MGCamara> mgCamara = std::dynamic_pointer_cast<MGCamara>(entidad);
        
        if (mgCamara) {
            // Si la conversión es exitosa, devuelve la cámara
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
