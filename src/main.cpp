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
#include "ParticleGenerator.hpp"
#include "ParticleGenerator3D.hpp"

// Camara camara(
//     glm::vec3(10.0f, 10.0f, 20.0f),
//     glm::vec3(0.0f, 0.0f, -1.0f),
//     glm::vec3(0.0f, 1.0f, 0.0f),
//     5.0f, 50.0f);

// Luz luz(
//     glm::vec3(10.0f, 10.0f, 20.0f),
//     glm::vec3(1.0f, 1.0f, 1.0f),
//     1.0f);


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


    Shader shaderPart("../shaders/vertex_particle.glsl", "../shaders/fragment_particle.glsl");
    Shader shaderPart3D("../shaders/vertex_particle3d.glsl", "../shaders/fragment_particle3d.glsl");

    Textura textura2D("../models/nicolas-cage-original.jpg");
    Fichero fichero("../models/prota.obj");
    Textura textura("../textures/prota.png");
    Textura textura2("../models/tex/hju.png");
    Fichero fichero2("../models/cubo.obj");
    Fichero fichero3("../models/karl_lowpoly.obj");

    Textura niebla("../models/niebla2.png");

    shaderPart.use();
    shaderPart3D.use();
    

    ParticleGenerator particleGenerator(&shaderPart, &niebla, 10000);
    ParticleGenerator3D particleGenerator3D(&shaderPart3D, &niebla, 10000000, 0.2f, 1.f, 1.f, 1.f, "cube", "cube");


    // MGMesh* entMALLA1 = mtg.crearMalla(mtg.getShader3D(), fichero, textura2, 0);
    
    std::vector<MGMesh*> entMALLAScomp = mtg.crearModeloComp(mtg.getShader3D(), fichero3);

    MGEntity* entVACIA = mtg.crearEntidadVacia(mtg.getShader3D());
    glm::vec3 tras{0.0f, 0.0f, 0.0f};
    glm::vec3 esc{1.0f, 1.0f, 1.0f};
    glm::vec3 rot{0.0f, 0.0f, 0.0f};


    // Nodo* nodomalla1 = mtg.crearNodo(mtg.getRaiz(),entMALLA1,tras,esc,rot);
    // nodomalla1->setEscalado(glm::vec3(1.0f));
    // nodomalla1->setTraslacion(glm::vec3(0.0f, 0.0f,0.0f));
    // nodomalla1->setRotacion(glm::vec3(0.0f, 90.0f,0.0f));

    // Nodo* nodomalla2 = mtg.crearNodo(mtg.getRaiz(),entMALLA2,tras,esc,rot);
    // nodomalla2->setEscalado(glm::vec3(0.01f));
    // nodomalla2->setTraslacion(glm::vec3(10.0f, 0.0f,0.0f));

    Nodo* nodoModeloPrueba = mtg.crearNodo(mtg.getRaiz(),entVACIA,tras,esc,rot);
    for(size_t i = 0; i<entMALLAScomp.size();i++){
        mtg.crearNodo(nodoModeloPrueba,entMALLAScomp[i],tras,esc,rot);
    }
    
    nodoModeloPrueba->setEscalado(glm::vec3(1.5f));
    nodoModeloPrueba->setTraslacion(glm::vec3(20.0f, 0.0f,0.0f));
    nodoModeloPrueba->setRotacion(glm::vec3(0.0f, -90.0f,0.0f));


    mtg.testMotorTAG();

    float lastTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    glm::vec2 pos(0.f, 0.f); // centro

    glm::vec3 pos3D(0.f, 0.f, 0.f); // centro

    float aceleratio = 5.f;
    float aceleratioy = -2.f;


    float contador = 0.f;
    mtg.pinta();
    
    while (!mtg.getWindow()->shouldClose()) {
        mtg.getWindow()->swapBuffers();
        mtg.getWindow()->pollEvents();

        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;


        // 1. Renderizar escena 3D
        mtg.init3D();
        mtg.dibujarEscena();
        
        mtg.end3D();

        // 2. Desactivar depth test para que las partículas 2D se vean encima

        // glEnable(GL_DEPTH_TEST);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // particleGenerator3D.Update(deltaTime, pos3D, {1.f,1.f,1.f}, 100, glm::vec3(10.0f));
        // particleGenerator3D.Draw(mtg.getCamaraActiva()->getViewMatrix());
        // glDisable(GL_DEPTH_TEST);

        // pos3D.y += 0.1f * aceleratioy;
        // pos3D.x += 0.1f * aceleratio;

        // if(pos3D.y <= -3.0f || pos3D.y >= 3.0f){
        //     aceleratioy *= -1.f;
        // }

        // if(pos3D.x <= -15.0f || pos3D.x >= 15.0f){
        //     aceleratio *= -1.f;
        // }

        mtg.DrawTexture(0.f, mtg.getWindow()->height, 800.0f, 600.0f, textura2D);
        mtg.DrawRectangle(0.0f, mtg.getWindow()->height, 400.0f, 600.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
        //particleGenerator.Draw2({200.f, mtg.getWindow()->height-300}, glm::vec4(1.0f, 1.0f, 1.0f, 1.f));
        // shaderPart.use();

        if(contador > 5.f)
        {           
            particleGenerator.Update(deltaTime, pos, {1.f,1.f}, 1, glm::vec2(10.0f));        
            contador = 0.f;
        } else{
            particleGenerator.Update(deltaTime, pos, {1.f,1.f}, 0, glm::vec2(10.0f));        
            contador += 1.f;
        }
        
        particleGenerator.Draw();

        // 4. (opcional) Volver a activar depth test si lo necesitas después
        glEnable(GL_DEPTH_TEST);


        // moverNodo(mtg.getWindow(),deltaTime, nodoluz);

        processInput(mtg.getWindow(), deltaTime, mtg);

        //mtg.init3D();
       // mtg.dibujarEscena();
        
        // mtg.DrawCube(nodoluz->getTraslacion().x, nodoluz->getTraslacion().y, nodoluz->getTraslacion().z, 1.0f, 1.0f, 1.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), mtg.getShader3D());
        //mtg.end3D();

        //mtg.DrawRectangle(200.0f, 500.0f, 100.0f, 100.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        // mtg.DrawRectangle(0.0f, 500.0f, 100.0f, 100.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        // mtg.DrawCircle(200.0f, 200.0f, 50.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        // mtg.DrawText("Hola Alex", 50.0f, 500.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        // mtg.DrawTexture(400.0f, 200.0f, 100.0f, 100.0f, textura2D);
    }
    return 0;
}
