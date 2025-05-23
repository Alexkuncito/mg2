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
#include "MeshAnimada.hpp"
#include "ModelAnimation.hpp"

// Camara camara(
//     glm::vec3(10.0f, 10.0f, 20.0f),
//     glm::vec3(0.0f, 0.0f, -1.0f),
//     glm::vec3(0.0f, 1.0f, 0.0f),
//     5.0f, 50.0f);

// Luz luz(
//     glm::vec3(10.0f, 10.0f, 20.0f),
//     glm::vec3(1.0f, 1.0f, 1.0f),
//     1.0f);

bool flag = false;
bool lastKey2State = false;

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

    bool currentKey2State = (glfwGetKey(glfwWindow, GLFW_KEY_2) == GLFW_PRESS);
    if(currentKey2State && !lastKey2State){
        flag=!flag;
    }
    lastKey2State = currentKey2State;

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
    flag = false;
    lastKey2State = false;


    TMotorTAG mtg;
    mtg.initWindow(800, 600, "Motor OpenGL 3D");

    mtg.init2D();
    mtg.LoadFont("../fonts/jack-of-gears.regular.ttf", 48);

    mtg.init3D();


    Fichero ficheroModelo("../models/karl_lowpoly.obj");
    Fichero ficheroModelo2("../models/prota.obj");
    Fichero ficheroModelo3("../textures/karl_lowpoly.obj");


    MGEntity* entVACIA = mtg.crearEntidadVacia(mtg.getShader3D());


    //MODELO KARL - COMPLETO - MATERIALES.
    glm::vec3 trasModel1{0.f, 0.f, 0.f};
    glm::vec3 escModel1{1.f, 1.f, 1.f};
    glm::vec3 rotModel1{0.f,-90.f,0.f};

    Nodo* nodoModeloPrueba = mtg.crearNodo(mtg.getRaiz(),entVACIA,trasModel1,escModel1,rotModel1);
    //std::vector<MGMesh*> entMALLAScomp = mtg.getENTCOMP("../models/karl_lowpoly.obj");
    std::vector<MGMesh*> entMALLAScomp = mtg.crearModeloComp(mtg.getShader3D(),ficheroModelo);

    for(size_t i = 0; i<entMALLAScomp.size();i++){
        mtg.crearNodo(nodoModeloPrueba,entMALLAScomp[i],{0.f,0.f,0.f},{1.f,1.f,1.f},{0.f,0.f,0.f});
    }


    //MODELO KARL - SEPARADO POR SUBMESHES.
    glm::vec3 trasModel2{0.f, 0.f, -5.f};
    glm::vec3 escModel2{1.f, 1.f, 1.f};
    glm::vec3 rotModel2{0.f,-90.f,0.f};

    for(size_t i = 0; i<entMALLAScomp.size();i++){
        mtg.crearNodo(mtg.getRaiz(),entMALLAScomp[i],trasModel2,escModel2,rotModel2);
        trasModel2.x += 5.0f;
    }


    //MODELO PROTA - TEXTURA
    glm::vec3 trasModel3{0.f, 0.f, -10.f};
    glm::vec3 escModel3{1.f, 1.f, 1.f};
    glm::vec3 rotModel3{0.f,-90.f,0.f};

    Nodo* nodoModeloPrueba2 = mtg.crearNodo(mtg.getRaiz(),entVACIA,trasModel3,escModel3,rotModel3);
    std::vector<MGMesh*> entMALLAScomp2 = mtg.crearModeloComp(mtg.getShader3D(),ficheroModelo2);

    for(size_t i = 0; i<entMALLAScomp2.size();i++){
        mtg.crearNodo(nodoModeloPrueba2,entMALLAScomp2[i],{0.f,0.f,0.f},{1.f,1.f,1.f},{0.f,0.f,0.f});
    }


    //SISTEMA DE PARTICULAS - STATIC
    glm::vec3 trasParticle{0.f, 0.f, -15.f};
    glm::vec3 escParticle{1.f, 1.f, 1.f};
    glm::vec3 rotParticle{0.f,-90.f,0.f};

    ParticleGenerator3D* Pg = mtg.crearGenParticulas(mtg.devTextura("../models/Nicolas_Cage_Deauville_2013.jpg"), 10000, 0.1f, 1.f,1.f,1.f,"cube","cube");

    //SISTEMA DE PARTICULAS - MOV
    glm::vec3 tras2Particle{0.f, 0.f, -20.f};
    glm::vec3 esc2Particle{1.f, 1.f, 1.f};
    glm::vec3 rot2Particle{0.f,-90.f,0.f};
    float vx = 0.2f;
    float vy = 0.2f;

    ParticleGenerator3D* Pg2 = mtg.crearGenParticulas(mtg.devTextura("../models/Niebla.png"), 5000, 0.1f, 0.5f,0.5f,0.5f,"sphere","star");

    //ANIMACION 3D
    glm::vec3 trasANIMATION1{0.f, 0.f, -25.f};
    glm::vec3 escANIMATION1{1.f, 1.f, 1.f};
    glm::vec3 rotANIMATION1{0.f,-90.f,0.f};


    ModelAnimation mAnim;
    mAnim.loadFromPath("../animacion/cubos_giratorios", &mtg);

    std::vector<MGMesh*> modelAni;

    Nodo* nodoModeloPrueba3 = mtg.crearNodo(mtg.getRaiz(),entVACIA,trasANIMATION1,escANIMATION1,rotANIMATION1);

    //RECURSO MATERIAL
    glm::vec3 trasmatREC{-5.f, 0.f, 0.f};
    glm::vec3 escmatREC{1.f, 1.f, 1.f};
    glm::vec3 rotmatREC{0.f,-90.f,0.f};


    TMaterial* prueba = mtg.devMaterial("../textures/karl_lowpoly.obj", 3);
    prueba->printMaterial();
    
    if(prueba)
    {
        MGMesh* malla = mtg.crearMalla(mtg.getShader3D(), ficheroModelo3);
        
        malla->setMaterial(*prueba);
        mtg.crearNodo(mtg.getRaiz(),malla,trasmatREC,escmatREC,rotmatREC);
    }
 

    // entMALLAScomp[0]->setMaterial(prueba);
    float lastTime = glfwGetTime();
    glEnable(GL_DEPTH_TEST);
    

    while (!mtg.getWindow()->shouldClose()) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        mtg.getWindow()->pollEvents();
        mtg.getWindow()->swapBuffers();

        processInput(mtg.getWindow(), deltaTime, mtg);

        if(flag){
            
        }

        //ANIMACION --ACTUALIZAMOS... YA PINTARÁ EL DIBUJAR ESCEN
        
        modelAni = mAnim.getCurrentFrame().mallas;

        for(size_t i = 0; i<modelAni.size();i++)
        {
            mtg.crearNodo(nodoModeloPrueba3,modelAni[i],{0.f,0.f,0.f},{1.f,1.f,1.f},{0.f,0.f,0.f});
        }

        //ACTUALIZAMOS SISTEMA DE PARTICULAS//

        //Movimiento del SISTEMA DE PARTICULAS 2
        tras2Particle.x += vx;
        tras2Particle.y += vy;
        if(tras2Particle.x >= 20.0f || tras2Particle.x <= -20.0f)
            vx*=-1;
        if(tras2Particle.y >= 5.0f || tras2Particle.y <= -5.0f)
        {
            vy*=-1;
        }

        mtg.updateParticleGen(Pg, trasParticle,{0.f,1.f,0.f}, 10, {0.f,0.f,0.f});
        mtg.updateParticleGen(Pg2, tras2Particle,{0.f,1.f,0.f}, 5, {0.f,0.f,0.f});


        mtg.initDrawing();
        mtg.init3D();
        mtg.dibujarEscena();
        auto hijos = nodoModeloPrueba3->getHijos();

        for (auto* hijo : hijos) {
            nodoModeloPrueba3->borrarHijo(hijo);
        }

        mAnim.updateFrame();
        mtg.end3D();
        mtg.closeDrawing();

}

    mtg.closeWindow();
    return 0;
}
