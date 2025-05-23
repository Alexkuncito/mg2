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
#include "gestorentidad.hpp"
#include "ParticleGenerator3D.hpp"

//==================================================
// Definición del motor principal TMotorTAG
//==================================================
struct TMotorTAG {
private:
    //==================================================
    // Recursos internos del motor
    //==================================================
    TGestorRecursos gestorRecursos;
    TGestorEntidades gestorEntidades;

    // std::unordered_map<int, std::unique_ptr<Nodo>> gnodos;
    // std::unordered_map<int, std::unique_ptr<MGEntity>> entidades;

    std::vector<std::unique_ptr<Nodo>> gnodos;
    std::vector<std::unique_ptr<MGEntity>> entidades;
    std::vector<std::unique_ptr<ParticleGenerator3D>> generadoresParticulas;

    Nodo* escena;
    int cantnode;

    std::vector<Camara> camv;

    std::vector<Nodo*> regCamaras;
    std::vector<Nodo*> regLuces;

    std::optional<Window> ventana;

    Camara camaraP = Camara(
        glm::vec3(0.0f, 5.0f, 5.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        5.0f, 50.0f);
    
    Luz luzP = Luz(
        glm::vec3(10.0f, 10.0f, 20.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f);

public:
    int valprueba = 0;

    TGestorRecursos* getGestorRecursos() {
        return &gestorRecursos;
    }



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

    ParticleGenerator3D* crearGenParticulas(Textura* textura, unsigned int amount, float particleScale, float horizontal, float vertical, float profundidad, std::string form, std::string particleForm);
    void updateParticleGen(ParticleGenerator3D* gen, glm::vec3 position, glm::vec3 velocity, unsigned int newParticles, glm::vec3 offset);
    
    void deleteParticleGen(ParticleGenerator3D* gen){
        auto it = std::find_if(generadoresParticulas.begin(), generadoresParticulas.end(),
            [gen](const std::unique_ptr<ParticleGenerator3D>& ptr) {
                return ptr.get() == gen;
            });

        if (it != generadoresParticulas.end()) {
            generadoresParticulas.erase(it);
        }
    }

    MGMesh* crearMalla(Shader* shader, const Fichero& fichero, 
                       std::optional<std::reference_wrapper<const Textura>> textura = std::nullopt, int val = 0);
    std::vector<MGMesh*> crearModeloComp(Shader* shader, const Fichero& fichero);

    MGCamara* crearCamara(Shader* shader, Camara* camara);

    Camara createCAM(float fovy,  glm::vec3 position, glm::vec3 target, glm::vec3 up, float speed, float sensitivity);

    MGLuz* crearLuz(Shader* shader, Luz* luz);

    void deleteCamara(MGCamara* cam);
    void deleteLuz(MGLuz* luz);
    void deleteMalla(MGMesh* mall);

    void resetEscena();

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
    Shader* getShader3DTrans();
    Shader* getShader3DPart();


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
    int MideTexto(std::string text, float scale);
    void cargaTotalOBJ(const std::string& path);


    //==================================================
    // Pruebas de MotorTAG
    //==================================================
    
#include <filesystem> // C++17


std::optional<std::reference_wrapper<const Textura>> devText(const Fichero& fichero, int i) {
    const std::string& texturaRelativa = getRutaFinal(fichero.getRuta(), fichero.getTextureRuta(i));
    ////std::cout << texturaRelativa << std::endl;


    Recurso* recursoTextura1 = gestorRecursos.getRecurso(texturaRelativa);
    auto* recursoTextura2D = dynamic_cast<RecursoTextura*>(recursoTextura1);

    if (!recursoTextura2D) {
        auto nuevoRecurso = std::make_unique<RecursoTextura>(texturaRelativa);
        recursoTextura2D = nuevoRecurso.get();
        gestorRecursos.add(std::move(nuevoRecurso));
    }

    if (recursoTextura2D) {
        return recursoTextura2D->returnTexturaRefW();  // Suponiendo que devuelve std::reference_wrapper<Textura>
    }

    return std::nullopt;
}

Textura* devTextura(const std::string& ruta){
    Recurso* recursoTextura1 = gestorRecursos.getRecurso(ruta);
    auto* recursoTextura2D = dynamic_cast<RecursoTextura*>(recursoTextura1);

    if (!recursoTextura2D) {
        auto nuevoRecurso = std::make_unique<RecursoTextura>(ruta);
        recursoTextura2D = nuevoRecurso.get();
        gestorRecursos.add(std::move(nuevoRecurso));
    }

    if (recursoTextura2D) {
        return recursoTextura2D->returnTextura();
    }

    return nullptr;
}

TMaterial* devMaterial(const std::string& ruta, int val){
    Recurso* recurso = gestorRecursos.getRecurso(ruta, val);
    auto* recursoM = dynamic_cast<RecursoMaterial*>(recurso);

    if (!recursoM) {
        std::cout<<"NO EXISTE"<<std::endl;
        auto nuevoRecurso = std::make_unique<RecursoMaterial>(ruta,val);
        recursoM = nuevoRecurso.get();
        gestorRecursos.add(std::move(nuevoRecurso));
    }

    if (recursoM) {
        return recursoM->returnMaterial();
    }


    return nullptr;
}


void testMotorTAG() {
    ////std::cout << "Prueba de MotorTAG" << std::endl;

    Fichero fichero("./assets/MotorTAG/models/prota.obj");
    Fichero fichero2("./assets/MotorTAG/models/karl_lowpoly.obj");
    MGMesh* entMALLA1 = nullptr;



    std::string texturaRelativa1 = fichero.getTextureRuta(0);
    
    if (!texturaRelativa1.empty()) {
        //std::cout << "Hay textura para malla 1" << std::endl;

        std::string texturaPath1 = "./assets/MotorTAG/models/" + texturaRelativa1;
        Recurso* recursoTextura1 = gestorRecursos.getRecurso(texturaPath1);
        RecursoTextura* recursoTextura2D_1 = dynamic_cast<RecursoTextura*>(recursoTextura1);

        if (!recursoTextura2D_1) {
            auto rct = std::make_unique<RecursoTextura>(texturaPath1);
            recursoTextura2D_1 = rct.get();
            gestorRecursos.add(std::move(rct));
        }

        auto textura2D_1 = recursoTextura2D_1->returnTexturaRefW();
        entMALLA1 = crearMalla(getShader3D(), fichero, textura2D_1, 0);
    } else {
        //std::cout << "No hay textura para malla 1" << std::endl;
        entMALLA1 = crearMalla(getShader3D(), fichero, std::nullopt, 0);
    }

    // --- Segunda malla (entMALLA2) ---

    MGEntity* entVACIA = crearEntidadVacia(getShader3D());
    
    std::vector<MGMesh*> entMALLAScomp = crearModeloComp(getShader3D(), fichero2);

    glm::vec3 tras{0.0f, 0.0f, 0.0f};
    glm::vec3 esc{1.0f, 1.0f, 1.0f};
    glm::vec3 rot{0.0f, 0.0f, 0.0f};


    Nodo* nodoModeloPrueba = crearNodo(getRaiz(),entVACIA,tras,esc,rot);
    for(size_t i = 0; i<entMALLAScomp.size();i++){
        crearNodo(nodoModeloPrueba,entMALLAScomp[i],tras,esc,rot);
    }
    
    nodoModeloPrueba->setEscalado(glm::vec3(1.5f));
    nodoModeloPrueba->setTraslacion(glm::vec3(20.0f, 0.0f,0.0f));
    nodoModeloPrueba->setRotacion(glm::vec3(0.0f, -90.0f,0.0f));



    // Nodo para la primera malla
    Nodo* nodomalla1 = crearNodo(getRaiz(), entMALLA1, tras, esc, rot);
    nodomalla1->setEscalado(glm::vec3(1.0f));
    nodomalla1->setTraslacion(glm::vec3(0.0f, 0.0f, 0.0f));
    nodomalla1->setRotacion(glm::vec3(0.0f, 180.0f, 0.0f));
}

void changeCamara(glm::vec3 startPos, glm::vec3 startFront, glm::vec3 startUp, float startSpeed, float startSensitivity){
    camaraP = Camara(startPos, startFront, startUp, startSpeed, startSensitivity);
}

void changeLuz(glm::vec3 startPos, glm::vec3 startColor, float startIntensity){
    luzP = Luz(startPos, startColor, startIntensity);
}

std::string getRutaFinal(const std::string& modeloRuta, const std::string& texturaRuta) {
    // 1. Buscar la última barra '/' o '\'
    size_t pos = modeloRuta.find_last_of("/\\");
    std::string carpetaModelo = (pos != std::string::npos) ? modeloRuta.substr(0, pos + 1) : "";

    // 2. Quitar el "./" inicial de la textura si existe
    std::string texturaRelativa = texturaRuta;
    if (texturaRuta.substr(0, 2) == "./") {
        texturaRelativa = texturaRuta.substr(2);
    }

    // 3. Concatenar
    return carpetaModelo + texturaRelativa;
}


std::vector<MGMesh*> getENTCOMP(const std::string& rutaBase) {
    return gestorEntidades.getCompEntidades(rutaBase);
}

};


#endif
