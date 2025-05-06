#ifndef MOTORTAG_HPP
#define MOTORTAG_HPP

#include "gestorrecursos.hpp"
#include "arbol.hpp"
#include "Window.hpp"
#include "Graphics2D.hpp"
#include "Graphics3D.hpp"
#include <optional>
#include <vector>
#include <memory> // Para shared_ptr

//==================================================
// Definición del motor principal TMotorTAG
//==================================================
struct TMotorTAG {
private:
    //==================================================
    // Recursos internos del motor
    //==================================================
    TGestorRecursos gestorRecursos;

    std::unordered_map<int, std::unique_ptr<Nodo>> gnodos;
    std::unordered_map<int, std::unique_ptr<MGEntity>> entidades;

    Nodo* escena;
    int cantnode;

    std::vector<Nodo*> regCamaras;
    std::vector<Nodo*> regLuces;

    std::optional<Window> ventana;


    Camara camaraP = Camara(
        glm::vec3(10.0f, 10.0f, 20.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        5.0f, 50.0f);
    
    Luz luzP = Luz(
        glm::vec3(10.0f, 10.0f, 20.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f);
public:
    //==================================================
    // Constructor y Destructor
    //==================================================
    TMotorTAG();
    ~TMotorTAG() = default;

    //==================================================
    // Creación de Nodos y Entidades
    //==================================================
    Nodo* crearNodo(Nodo* padre, MGEntity* ent, glm::vec3 traslacion, glm::vec3 escalado, glm::vec3 rotacion);
    Nodo* crearNodoModelo(Nodo* padre, std::vector<MGMesh> ent, glm::vec3 traslacion, glm::vec3 escalado, glm::vec3 rotacion);

    MGEntity* crearEntidadVacia(Shader* shader);
    MGMesh* crearMalla(Shader* shader, const Fichero& fichero, 
                       std::optional<std::reference_wrapper<const Textura>> textura = std::nullopt, int val = 0);
    std::vector<MGMesh*> crearModeloComp(Shader* shader, const Fichero& fichero, 
                                         std::optional<std::reference_wrapper<const Textura>> textura = std::nullopt);

    MGCamara* crearCamara(Shader* shader, Camara* camara);
    MGLuz* crearLuz(Shader* shader, Luz* luz);

    void deleteCamara(MGCamara* cam);
    void deleteLuz(MGLuz* luz);
    void deleteMalla(MGMesh* mall);

    //==================================================
    // Gestión de Cámaras y Luces
    //==================================================
    int registrarCamara(Nodo* cam);
    int registrarLuz(Nodo* luz);

    void activarCamara(int v);
    void activarLuz(int v);

    void desActivarCamara(int v);
    void desActivarLuz(int v);

    Camara* getCamaraActiva();

    //==================================================
    // Renderizado de Escena
    //==================================================
    void dibujarEscena();
    void pinta();

    Nodo* getRaiz() { return escena; }

    //==================================================
    // Inicialización y Finalización de Contextos
    //==================================================
    void init3D();
    void end3D();
    Shader* getShader3D();

    void init2D();
    void end2D();

    //==================================================
    // Gestión de Ventana
    //==================================================
    void initWindow(int const w, int const h, char const* title);
    void closeWindow();
    Window* getWindow();
    bool WindowIsOpen();

    //==================================================
    // Ciclo de Dibujo General
    //==================================================
    void clearBackground(float r, float g, float b, float a);
    void initDrawing(float r = 0.1f, float g = 0.1f, float b = 0.1f, float a = 1.0f);
    void closeDrawing();

    //==================================================
    // Funciones de Dibujo 3D
    //==================================================
    void DrawCube(float x, float y, float z, float width, float height, float depth, glm::vec4 color, Shader* shader);

    //==================================================
    // Funciones de Dibujo 2D
    //==================================================
    void DrawRectangle(float x, float y, float width, float height, glm::vec4 color);
    void DrawCircle(float x, float y, float radius, glm::vec4 color);
    void LoadFont(const std::string& fontPath, unsigned int fontSize);
    void DrawText(std::string text, float x, float y, float scale, glm::vec3 color);
    void DrawTexture(float x, float y, float width, float height, const Textura& texture);

    //==================================================
    // Pruebas de MotorTAG
    //==================================================
    void testMotorTAG(){
        init3D();
        std::cout << "Prueba de MotorTAG" << std::endl;
        MGCamara* entCAMARA = crearCamara(getShader3D(), &camaraP);
        MGLuz* entLUZ = crearLuz(getShader3D(), &luzP);

        glm::vec3 tras{0.0f, 0.0f, 0.0f};
        glm::vec3 esc{1.0f, 1.0f, 1.0f};
        glm::vec3 rot{0.0f, 0.0f, 0.0f};

        Nodo* nodocamara = crearNodo(getRaiz(),entCAMARA,tras,esc,rot);
        Nodo* nodoluz = crearNodo(getRaiz(),entLUZ,tras,esc,rot);

        registrarCamara(nodocamara);
        registrarLuz(nodoluz);
    }

};

#endif
