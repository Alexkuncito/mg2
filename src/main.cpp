#define GLM_ENABLE_EXPERIMENTAL
#include "arbol.hpp"
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

void processInput(Window& window, float deltaTime, TMotorTAG& mtg) {
    GLFWwindow* glfwWindow = window.getNativeWindow();
    
    // Obtener la cámara activa del árbol de MTG
    std::shared_ptr<Camara> camaraActiva = mtg.getCamaraActiva();
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


std::shared_ptr<Shader> shader3D;
std::shared_ptr<Shader> shader2D;

void init2D() {
    shader2D = std::make_shared<Shader>("../shaders/vertex_2d.glsl", "../shaders/fragment_2d.glsl");
}

void init3D() {
    shader3D = std::make_shared<Shader>("../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");
}

int main() {
    Window window(800, 600, "Motor OpenGL 3D");

    init3D();
    Graphics2D::init2D();
    Graphics2D::LoadFont("../fonts/jack-of-gears.regular.ttf", 48);

    TMotorTAG mtg(shader3D);
    TGestorRecursos rec;
    rec.cargarMateriales();

    float fov = glm::radians(45.0f);
    glm::mat4 projection = glm::perspective(fov, (float)window.width / (float)window.height, 0.1f, 100.0f);

    Textura textura2D("../textures/enemigo.png");

    //rec.ImprimirRecursos();


    Fichero fichero("../models/prota.obj");
    Textura textura("../textures/prota.png");

    std::shared_ptr<RecursoMaterial> mat1 = std::dynamic_pointer_cast<RecursoMaterial>(rec.getRecurso("../material/materialGold.txt"));
    TMaterial* rawMaterial = mat1->returnMaterial().get();
    TMaterial& prueba = *rawMaterial;
    TMaterial pruebaconv = prueba;

    std::shared_ptr<RecursoMaterial> mat2 = std::dynamic_pointer_cast<RecursoMaterial>(rec.getRecurso("../material/materialPearl.txt"));
    TMaterial* rawMaterial2 = mat2->returnMaterial().get();
    TMaterial& prueba2 = *rawMaterial2;
    TMaterial pruebaconv2 = prueba2;


    Mesh mesh(fichero, nullopt, prueba);

    // Uso de shared_ptr para las entidades

    Fichero fichero2("../models/cubo.obj");
    Mesh mesh2(fichero2, textura, prueba2);

    //std::shared_ptr<MGMesh> entMALLA1 = std::make_shared<MGMesh>(shader3D, std::make_shared<Mesh>(mesh));
    std::shared_ptr<MGMesh> entMALLA1 = mtg.crearMalla(shader3D, fichero, nullopt, pruebaconv);

    
    //std::shared_ptr<MGEntity> entMALLA2 = std::make_shared<MGMesh>(std::make_shared<Shader>(*shader3D), std::make_shared<Mesh>(mesh2));
    std::shared_ptr<MGEntity> entMALLA2 = mtg.crearMalla(shader3D, fichero2, nullopt, pruebaconv2);


    std::shared_ptr<MGEntity> entCAMARA = std::make_shared<MGCamara>(shader3D, std::make_shared<Camara>(camara));
    std::shared_ptr<MGEntity> entLUZ = std::make_shared<MGLuz>(shader3D, std::make_shared<Luz>(luz));
    std::shared_ptr<MGEntity> entVACIA = std::make_shared<MGEntity>(shader3D);

    //std::shared_ptr<MGEntity> entMALLA1gr = mtg.crearMalla();
    // Creación de nodos con shared_ptr
    std::shared_ptr<Nodo> raiz = std::make_shared<Nodo>(entVACIA, 0);
    std::shared_ptr<Nodo> hijo1 = std::make_shared<Nodo>(entMALLA1, 1);
    std::shared_ptr<Nodo> hijo2 = std::make_shared<Nodo>(entMALLA2, 2);
    std::shared_ptr<Nodo> camaraNodo = std::make_shared<Nodo>(entCAMARA, 3);
    std::shared_ptr<Nodo> luzNodo = std::make_shared<Nodo>(entLUZ, 4);

    glm::vec3 tras{0.0f, 0.0f, 0.0f};
    glm::vec3 esc{1.0f, 1.0f, 1.0f};
    glm::vec3 rot{0.0f, 0.0f, 0.0f};


    std::shared_ptr<Nodo> nodomalla1 = mtg.crearNodo(mtg.getRaiz(),entMALLA1,tras,esc,rot);
    nodomalla1.get()->setEscalado(glm::vec3(0.01f));
    nodomalla1.get()->setRotacion(glm::vec3(0.0f, 90.0f,0.0f));

    std::shared_ptr<Nodo> nodomalla2 = mtg.crearNodo(mtg.getRaiz(),entMALLA2,tras,esc,rot);
    nodomalla2.get()->setEscalado(glm::vec3(0.01f));

    std::shared_ptr<Nodo> nodocamara = mtg.crearNodo(mtg.getRaiz(),entCAMARA,tras,esc,rot);
    std::shared_ptr<Nodo> nodoluz = mtg.crearNodo(mtg.getRaiz(),entLUZ,tras,esc,rot);

    int nCamara = mtg.registrarCamara(nodocamara);

    int nLuz = mtg.registrarLuz(nodoluz);

    // Construcción de la jerarquía del árbol
    raiz->agregarHijo(hijo1);
    raiz->agregarHijo(hijo2);
    raiz->agregarHijo(camaraNodo);
    raiz->agregarHijo(luzNodo);

    // Aplicar transformaciones
    hijo2->setTraslacion(glm::vec3(1.0f, 0.0f, 0.0f));
    hijo1->setEscalado(glm::vec3(0.005f));
    hijo2->setEscalado(glm::vec3(0.01f));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.005f));

    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::scale(model2, glm::vec3(0.005f));
    model2 = glm::translate(model2, glm::vec3(30.0f, 20.0f, 10.0f));

    float lastTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    // imprimirArbol(raiz,0);
    mtg.pinta();
    
    while (!window.shouldClose()) {
        shader3D->use();
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(window, deltaTime, mtg);

        shader3D->setMat4("projection", projection);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Recorrer el árbol y dibujar los nodos    
        raiz->recorrer(glm::mat4(1.0f));


        model = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f));
        shader3D->setMat4("model", model);
        Graphics3D::DrawCube(10.0f, 0.0f, 0.0f, 5.0f, 5.0f, 5.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), shader3D.get());

        mtg.dibujarEscena();

        glDisable(GL_DEPTH_TEST);

        Graphics2D::DrawRectangle(200.0f, 500.0f, 100.0f, 100.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        Graphics2D::DrawRectangle(0.0f, 500.0f, 100.0f, 100.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        Graphics2D::DrawCircle(200.0f, 200.0f, 50.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        Graphics2D::DrawText("Hola Alex", 50.0f, 500.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        Graphics2D::DrawTexture(400.0f, 200.0f, 100.0f, 100.0f, textura2D);

        glBindVertexArray(0);
        
        shader3D->use();
        shader3D->setMat4("projection", projection);

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}
