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

void processInput(Window& window, float deltaTime) {
    GLFWwindow* glfwWindow = window.getNativeWindow();

    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
        camara.moveForward(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
        camara.moveBackward(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
        camara.moveLeft(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
        camara.moveRight(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_UP) == GLFW_PRESS)
        camara.zoomIn(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
        camara.zoomOut(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
        camara.rotateLeft(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camara.rotateRight(deltaTime);
}

std::unique_ptr<Shader> shader3D;
std::unique_ptr<Shader> shader2D;

void init2D() {
    shader2D = std::make_unique<Shader>("../shaders/vertex_2d.glsl", "../shaders/fragment_2d.glsl");
 }

void init3D() {
    shader3D = std::make_unique<Shader>("../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");
}

int main() {
    Window window(800, 600, "Motor OpenGL 3D");

    //Shader shader3D("../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");
    init3D();
    //Shader shader2d("../shaders/vertex_2d.glsl", "../shaders/fragment_2d.glsl");
    Graphics2D::init2D();
    Graphics2D::LoadFont("../fonts/jack-of-gears.regular.ttf", 48);

    TGestorRecursos rec;

    rec.cargarMateriales();

    rec.ImprimirRecursos();

    float fov = glm::radians(45.0f);
    glm::mat4 projection = glm::perspective(fov, (float) window.width / (float) window.height, 0.1f, 100.0f);

    RecursoMaterial* mat1 = dynamic_cast<RecursoMaterial*>(rec.getRecurso("Cyanrubber"));
    Textura textura2D("../textures/enemigo.png");

    Fichero fichero("../models/prota.obj");
    Textura textura("../textures/prota.png");
    Mesh mesh(fichero,nullopt, mat1->returnMaterial());
    MGMesh ent1(shader3D.get(), &mesh);

    Fichero fichero2("../models/cubo.obj");
    Mesh mesh2(fichero2, textura);
    MGMesh ent2(shader3D.get(), &mesh2);

    MGCamara ent3(shader3D.get(), &camara);
    MGLuz ent4(shader3D.get(), &luz);

    MGEntity ent0(shader3D.get());
    // Creación de nodos
    Nodo* raiz = new Nodo(&ent0,0);
    Nodo* hijo1 = new Nodo(&ent1,1);
    Nodo* hijo2 = new Nodo(&ent2, 2);
    Nodo* camara = new Nodo(&ent3, 3);
    Nodo* luze = new Nodo(&ent4, 4);;
    // Construcción de la jerarquía del árbol
    raiz->agregarHijo(hijo1);
    raiz->agregarHijo(hijo2);
    raiz->agregarHijo(camara);
    raiz->agregarHijo(luze);

    //Aplicar transformaciones
    //raiz->setRotacion(glm::vec3(0.0f, 45.0f, 0.0f));
    //hijo1->setEscalado(glm::vec3(1.5f, 1.5f, 1.5f));
    hijo2->setTraslacion(glm::vec3(1.0f, 0.0f, 0.0f));
    hijo1->setEscalado(glm::vec3(0.005f));
    hijo2->setEscalado(glm::vec3(0.01f));
    
    // Imprimir estructura del árbol
    // cout << "Estructura del árbol:" << endl;
    // imprimirArbol(raiz);   

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.005f));

    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::scale(model2, glm::vec3(0.005f));
    model2 = glm::translate(model2, glm::vec3(30.0f, 20.0f, 10.0f));

    float lastTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);
    while (!window.shouldClose()) {
        shader3D->use();
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(window, deltaTime);
        //luz.aplicar(*shader3D);

        // glm::mat4 view = camara.getViewMatrix();
        // shader3D->setMat4("view", view);
        shader3D->setMat4("projection", projection);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Recorrer el árbol y dibujar los nodos    
        //cout << "Recorriendo el árbol y dibujando nodos:" << endl;
        raiz->recorrer(glm::mat4(1.0f));
        //ent2.draw(model2);

        model = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f));
        shader3D->setMat4("model", model);
        Graphics3D::DrawCube(10.0f, 0.0f, 0.0f, 5.0f, 5.0f, 5.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), shader3D.get());

        glDisable(GL_DEPTH_TEST);
        //shader2D->use();
        //shader2D->setMat4("projection", orthoProjection);
        Graphics2D::DrawRectangle(200.0f, 500.0f, 100.0f, 100.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        Graphics2D::DrawRectangle(0.0f, 500.0f, 100.0f, 100.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        Graphics2D::DrawCircle(200.0f, 200.0f, 50.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        Graphics2D::DrawText("Hola Alex", 50.0f, 500.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        Graphics2D::DrawTexture(400.0f, 200.0f, 100.0f, 100.0f, textura2D);

        glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);
        shader3D->use();
        shader3D->setMat4("projection", projection);

        window.swapBuffers();
        window.pollEvents();
    }

    // Liberar memoria
    delete raiz;
    return 0;
}