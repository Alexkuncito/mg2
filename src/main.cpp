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
    glm::vec3(0.0f, 12.0f, 15.0f),
    glm::vec3(0.0f, 0.0f, -1.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    5.0f, 50.0f);

Luz luz(
    glm::vec3(5.0f, 10.0f, 5.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    1.0f);

//Esto en motor TAG
void processInput(Window& window, float deltaTime, TMotorTAG& mtg) { //TMotorTAG& mtg
    GLFWwindow* glfwWindow = window.getNativeWindow();
    
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

//Esto en motor TAG o Graphics3D
std::unique_ptr<Shader> shader3D;

//Esto en motor TAG o Graphics3D
void init3D() {
    shader3D = std::make_unique<Shader>("../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");
}

int main() {
    //Esto en motor TAG como crearVentana(ancho, alto, nombre)
    Window window(800, 600, "Motor OpenGL 3D");

    init3D();

    //Esto en motor TAG como init2D
    Graphics2D::init2D();

    //Esto en motor TAG como cargarFuente(fuente, tamaño)
    Graphics2D::LoadFont("../fonts/jack-of-gears.regular.ttf", 48);

    //INSANE hay que ver que cambiar y como
    
    TMotorTAG mtg(shader3D.get());
    TGestorRecursos rec;
    rec.cargarMateriales();
    rec.ImprimirRecursos();

    //Esto en motor TAG como setProyeccion(fov)
    float fov = glm::radians(45.0f);
    glm::mat4 projection = glm::perspective(fov, (float)window.width / (float)window.height, 0.1f, 100.0f);

    //Todo el manejo este de cosas en el motor TAG para cargar y crear meshes y todo ese rollo
    Textura textura2D("../textures/enemigo.png");
    Fichero fichero("../models/prota.obj");
    Textura textura("../textures/prota.png");

    Recurso* recurso = rec.getRecurso("../material/materialGold.txt");
    RecursoMaterial* mat1 = dynamic_cast<RecursoMaterial*>(recurso);
    TMaterial* matr1 = mat1->returnMaterial();
    

    Recurso* recurso2 = rec.getRecurso("../material/materialJade.txt");
    RecursoMaterial* mat2 = dynamic_cast<RecursoMaterial*>(recurso2);
    TMaterial* matr2 = mat2->returnMaterial();

    Mesh mesh(fichero, nullopt, *matr1);

    Fichero fichero2("../models/cubo.obj");
    Mesh mesh2(fichero2, nullopt, *matr2);

    //std::unique_ptr<MGEntity> entMALLA1 = std::make_unique<MGMesh>(shader3D.get(), &mesh);
    MGMesh entMALLA1 = mtg.crearMalla(shader3D.get(), fichero, nullopt, *matr1);

    //std::shared_ptr<MGEntity> entMALLA2 = std::make_shared<MGMesh>(std::make_shared<Shader>(*shader3D), std::make_shared<Mesh>(mesh2));
    //std::shared_ptr<MGEntity> entMALLA2 = mtg.crearMalla(shader3D, fichero2, nullopt, pruebaconv2);

    //std::unique_ptr<MGEntity> entMALLA2 = std::make_unique<MGMesh>(shader3D.get(), &mesh2);
    MGMesh entMALLA2 = mtg.crearMalla(shader3D.get(), fichero2, nullopt, *matr2);


    // std::shared_ptr<MGEntity> entCAMARA = std::make_shared<MGCamara>(shader3D, std::make_shared<Camara>(camara));
    // std::shared_ptr<MGEntity> entLUZ = std::make_shared<MGLuz>(shader3D, std::make_shared<Luz>(luz));
    // std::shared_ptr<MGEntity> entVACIA = std::make_shared<MGEntity>(shader3D);

    //std::unique_ptr<MGEntity> entCAMARA = std::make_unique<MGCamara>(shader3D.get(), &camara);
    //std::unique_ptr<MGEntity> entLUZ = std::make_unique<MGLuz>(shader3D.get(), &luz);
    //std::unique_ptr<MGEntity> entVACIA = std::make_unique<MGEntity>(shader3D.get());

    MGCamara entCAMARA = mtg.crearCamara(shader3D.get(), &camara);
    MGLuz entLUZ = mtg.crearLuz(shader3D.get(), &luz);

    std::unique_ptr<MGEntity> entVACIA = std::make_unique<MGEntity>(shader3D.get());



    //std::shared_ptr<MGEntity> entMALLA1gr = mtg.crearMalla();
    // Creación de nodos con shared_ptr
    std::unique_ptr<Nodo> raiz = std::make_unique<Nodo>(entVACIA.get());
    std::unique_ptr<Nodo> hijo1 = std::make_unique<Nodo>(&entMALLA1);
    std::unique_ptr<Nodo> hijo2 = std::make_unique<Nodo>(&entMALLA2);
    std::unique_ptr<Nodo> camaraNodo = std::make_unique<Nodo>(&entCAMARA);
    std::unique_ptr<Nodo> luzNodo = std::make_unique<Nodo>(&entLUZ);

    glm::vec3 tras{0.0f, 0.0f, 0.0f};
    glm::vec3 esc{1.0f, 1.0f, 1.0f};
    glm::vec3 rot{0.0f, 0.0f, 0.0f};


    Nodo* nodomalla1 = mtg.crearNodo(mtg.getRaiz(),&entMALLA1,tras,esc,rot);
    nodomalla1->setEscalado(glm::vec3(0.01f));
    nodomalla1->setRotacion(glm::vec3(0.0f, 90.0f,0.0f));

    Nodo* nodomalla2 = mtg.crearNodo(mtg.getRaiz(),&entMALLA2,tras,esc,rot);
    nodomalla2->setEscalado(glm::vec3(0.01f));

    Nodo* nodocamara = mtg.crearNodo(mtg.getRaiz(),&entCAMARA,tras,esc,rot);
    Nodo* nodoluz = mtg.crearNodo(mtg.getRaiz(),&entLUZ,tras,esc,rot);

    int nCamara = mtg.registrarCamara(nodocamara);

    int nLuz = mtg.registrarLuz(nodoluz);

    // Construcción de la jerarquía del árbol
    raiz->agregarHijo(hijo1.get());
    raiz->agregarHijo(hijo2.get());
    raiz->agregarHijo(camaraNodo.get());
    raiz->agregarHijo(luzNodo.get());

    // Aplicar transformaciones
    hijo2->setTraslacion(glm::vec3(1.0f, 0.0f, 0.0f));
    hijo1->setEscalado(glm::vec3(2.0f));
    hijo2->setEscalado(glm::vec3(2.0f));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.005f));

    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::scale(model2, glm::vec3(0.005f));
    model2 = glm::translate(model2, glm::vec3(30.0f, 20.0f, 10.0f));

    float lastTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    imprimirArbol(raiz.get(),0);
    mtg.pinta();
    
    while (!window.shouldClose()) {
        //No se como pasar esto al motor TAG (todo lo de el window.shouldClose() y lo de abajo)
        window.swapBuffers();
        window.pollEvents();

        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(window, deltaTime, mtg);

        //Esto se deberá hacer cada vez que se quiera pintar algo 3D
        shader3D->use();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        shader3D->setMat4("projection", projection);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mtg.dibujarEscena();

        // entCAMARA->draw(glm::mat4(1.0f));
        // entLUZ->draw(glm::mat4(1.0f));
        // entMALLA1->draw(glm::mat4(1.0f));
        // entMALLA2->draw(glm::mat4(1.0f));

        //raiz->recorrer(glm::mat4(1.0f));

        //Esto se tiene que hacer en el motor TAG de alguna manera no tengo ni idea
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        shader3D->setMat4("model", model);
        Graphics3D::DrawCube(10.0f, 0.0f, 0.0f, 5.0f, 5.0f, 5.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), shader3D.get());

        // //Cada uno de estos tendrán sus funciones equivalentes en el motor TAG
        Graphics2D::DrawRectangle(200.0f, 500.0f, 100.0f, 100.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        Graphics2D::DrawRectangle(0.0f, 500.0f, 100.0f, 100.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        Graphics2D::DrawCircle(200.0f, 200.0f, 50.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        Graphics2D::DrawText("Hola Alex", 50.0f, 500.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        Graphics2D::DrawTexture(400.0f, 200.0f, 100.0f, 100.0f, textura2D);
    }

    return 0;
}
