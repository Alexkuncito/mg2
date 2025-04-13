#define GLM_ENABLE_EXPERIMENTAL
#include "arbol.hpp"
#include "entidad.hpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Camara.hpp"
#include "Luz.hpp"
#include "Fichero.hpp"
#include "Textura.hpp"
#include "Graphics2D.hpp"
#include "Graphics3D.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "recurso.hpp"
#include "MaterialRecurso.hpp"
#include "gestorrecursos.hpp"
#include "TexturaRecurso.hpp"
#include "motortag.hpp"
#include <vector>
#include <glm/gtx/string_cast.hpp>
#include <memory>

Camara camara(
    glm::vec3(10.0f, 10.0f, 20.0f),
    glm::vec3(0.0f, 0.0f, -1.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    5.0f, 50.0f);

Luz luz(
    glm::vec3(10.0f, 10.0f, 20.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    1.0f);


void moverNodo(Window* window, float deltaTime, Nodo* nodo) {
    GLFWwindow* glfwWindow = window->getNativeWindow();
    
    glm::vec3 traslacionActual = nodo->getTraslacion();  // Obtiene la traslación actual del nodo
    glm::vec3 movimiento(0.0f);  // Inicializa el movimiento en 0

    // Detectar teclas numéricas para mover el nodo
    if (glfwGetKey(glfwWindow, GLFW_KEY_1) == GLFW_PRESS) {
        movimiento.z -= deltaTime;  // Mover hacia adelante en el eje Z (hacia adelante)
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_2) == GLFW_PRESS) {
        movimiento.z += deltaTime;  // Mover hacia atrás en el eje Z (hacia atrás)
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_3) == GLFW_PRESS) {
        movimiento.x += deltaTime;  // Mover hacia la derecha en el eje X
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_4) == GLFW_PRESS) {
        movimiento.x -= deltaTime;  // Mover hacia la izquierda en el eje X
    }

    // Actualizar la traslación del nodo
    nodo->setTraslacion(traslacionActual + movimiento);
}



//Esto en motor TAG
void processInput(Window* window, float deltaTime, TMotorTAG& mtg) { //TMotorTAG& mtg
    GLFWwindow* glfwWindow = window->getNativeWindow();
    
    if (glfwGetKey(glfwWindow, GLFW_KEY_O) == GLFW_PRESS)
        window->close();
        
    // Obtener la cámara activa del árbol de MTG
    Camara* camaraActiva = mtg.getCamaraActiva();
    if (!camaraActiva) return;
    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
        camaraActiva->moveForward(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
        camaraActiva->moveBackward(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
        camaraActiva->moveLeft(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
        camaraActiva->moveRight(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_UP) == GLFW_PRESS)
        camaraActiva->zoomIn(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
        camaraActiva->zoomOut(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
        camaraActiva->rotateLeft(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camaraActiva->rotateRight(deltaTime);
}

int main() {
    TMotorTAG mtg;
    mtg.initWindow(800, 600, "Motor OpenGL 3D");

    mtg.init2D();
    mtg.LoadFont("../fonts/jack-of-gears.regular.ttf", 48);
    mtg.init3D();

    Textura textura2D("../textures/enemigo.png");
    Fichero fichero("../models/prota.obj");
    Textura textura("../textures/prota.png");
    Textura textura2("../models/"+fichero.getTextureRuta(0));
    Fichero fichero2("../models/cubo.obj");
    Fichero fichero3("../models/karl_lowpoly.obj");
    
    MGMesh* entMALLA1 = mtg.crearMalla(mtg.getShader3D(), fichero, textura2, 0);
    
    std::vector<MGMesh*> entMALLAScomp = mtg.crearModeloComp(mtg.getShader3D(), fichero3);
    MGMesh* entMALLA2 = mtg.crearMalla(mtg.getShader3D(), fichero2);

    MGCamara* entCAMARA = mtg.crearCamara(mtg.getShader3D(), &camara);
    MGLuz* entLUZ = mtg.crearLuz(mtg.getShader3D(), &luz);

    MGEntity* entVACIA = mtg.crearEntidadVacia(mtg.getShader3D());
    glm::vec3 tras{0.0f, 0.0f, 0.0f};
    glm::vec3 esc{1.0f, 1.0f, 1.0f};
    glm::vec3 rot{0.0f, 0.0f, 0.0f};


    Nodo* nodomalla1 = mtg.crearNodo(mtg.getRaiz(),entMALLA1,tras,esc,rot);
    nodomalla1->setEscalado(glm::vec3(1.0f));
    nodomalla1->setTraslacion(glm::vec3(0.0f, 0.0f,0.0f));
    nodomalla1->setRotacion(glm::vec3(0.0f, 90.0f,0.0f));

    Nodo* nodomalla2 = mtg.crearNodo(mtg.getRaiz(),entMALLA2,tras,esc,rot);
    nodomalla2->setEscalado(glm::vec3(0.01f));
    nodomalla2->setTraslacion(glm::vec3(10.0f, 0.0f,0.0f));

    Nodo* nodoModeloPrueba = mtg.crearNodo(mtg.getRaiz(),entVACIA,tras,esc,rot);
    for(size_t i = 0; i<entMALLAScomp.size();i++){
        mtg.crearNodo(nodoModeloPrueba,entMALLAScomp[i],tras,esc,rot);
    }
    
    nodoModeloPrueba->setEscalado(glm::vec3(1.5f));
    nodoModeloPrueba->setTraslacion(glm::vec3(20.0f, 0.0f,0.0f));
    nodoModeloPrueba->setRotacion(glm::vec3(0.0f, -90.0f,0.0f));

    Nodo* nodocamara = mtg.crearNodo(mtg.getRaiz(),entCAMARA,tras,esc,rot);
    Nodo* nodoluz = mtg.crearNodo(mtg.getRaiz(),entLUZ,tras,esc,rot);

    mtg.registrarCamara(nodocamara);

    mtg.registrarLuz(nodoluz);

    float lastTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    mtg.pinta();
    
    while (!mtg.getWindow()->shouldClose()) {
        mtg.getWindow()->swapBuffers();
        mtg.getWindow()->pollEvents();

        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;


        moverNodo(mtg.getWindow(),deltaTime, nodoluz);

        processInput(mtg.getWindow(), deltaTime, mtg);

        mtg.init3D();
        mtg.dibujarEscena();
        
        mtg.DrawCube(nodoluz->getTraslacion().x, nodoluz->getTraslacion().y, nodoluz->getTraslacion().z, 1.0f, 1.0f, 1.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), mtg.getShader3D());
        mtg.end3D();

        mtg.DrawRectangle(200.0f, 500.0f, 100.0f, 100.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        mtg.DrawRectangle(0.0f, 500.0f, 100.0f, 100.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        mtg.DrawCircle(200.0f, 200.0f, 50.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        mtg.DrawText("Hola Alex", 50.0f, 500.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        mtg.DrawTexture(400.0f, 200.0f, 100.0f, 100.0f, textura2D);
    }

    return 0;
}
