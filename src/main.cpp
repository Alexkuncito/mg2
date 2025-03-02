#include "arbol.cpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Camara.hpp"
#include "Luz.hpp"
#include "Fichero.hpp"
#include "Textura.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "recurso.hpp"
#include "MaterialRecurso.hpp"
#include "gestorrecursos.hpp"
#include <vector>

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

int main() {
    Window window(800, 600, "Motor OpenGL 3D");

    Shader shader("../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");

    TGestorRecursos rec;

    RecursoMaterial materialOro("oro", TMaterial
    ({0.24725f,0.1995f,0.0745f},{0.75164f,0.60648f,0.22648f},{0.628281f,0.555802f,0.366065f},0.4f));

    rec.add(&materialOro);

    RecursoMaterial materialEsmeralda("esmeralda", TMaterial
    ({0.0215f, 0.1745f, 0.0215f}, {0.07568f, 0.61424f, 0.07568f}, {0.633f, 0.727811f, 0.633f}, 0.6f));

    rec.add(&materialEsmeralda);

    RecursoMaterial materialJade("jade", TMaterial
    ({0.135f, 0.2225f, 0.1575f}, {0.54f, 0.89f, 0.63f}, {0.316228f, 0.316228f, 0.316228f}, 0.1f));

    rec.add(&materialJade);

    RecursoMaterial materialObsidiana("obsidiana", TMaterial
    ({0.05375f, 0.05f, 0.06625f}, {0.18275f, 0.17f, 0.22525f}, {0.332741f, 0.328634f, 0.346435f}, 0.3f));

    rec.add(&materialObsidiana);

    RecursoMaterial materialPerla("perla", TMaterial
    ({0.25f, 0.20725f, 0.20725f}, {1.0f, 0.829f, 0.829f}, {0.296648f, 0.296648f, 0.296648f}, 0.088f));

    rec.add(&materialPerla);

    RecursoMaterial materialRuby("rubí", TMaterial
    ({0.1745f, 0.01175f, 0.01175f}, {0.61424f, 0.04136f, 0.04136f}, {0.727811f, 0.626959f, 0.626959f}, 0.6f));

    rec.add(&materialRuby);

    rec.ImprimirRecursos();

    float fov = glm::radians(45.0f);
    glm::mat4 projection = glm::perspective(fov, 800.0f / 600.0f, 0.1f, 100.0f);

    Fichero fichero("../models/prota.obj");
    Textura textura("../textures/prota.png");
    Mesh mesh(fichero,nullopt, materialObsidiana.GetMaterial());
    MGMesh ent1(&shader, &mesh);

    Fichero fichero2("../models/cubo.obj");
    //Textura textura2("../textures/ladrillo.png");
    Mesh mesh2(fichero2, textura, materialJade.GetMaterial());
    MGMesh ent2(&shader, &mesh2);

    MGEntity ent0(&shader);
    // Creación de nodos
    Nodo* raiz = new Nodo(&ent0,0);
    Nodo* hijo1 = new Nodo(&ent1,1);
    Nodo* hijo2 = new Nodo(&ent2, 2);
    // Construcción de la jerarquía del árbol
    raiz->agregarHijo(hijo1);
    raiz->agregarHijo(hijo2);
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
    shader.use();
    while (!window.shouldClose()) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(window, deltaTime);
        luz.aplicar(shader);

        glm::mat4 view = camara.getViewMatrix();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Recorrer el árbol y dibujar los nodos
        //cout << "Recorriendo el árbol y dibujando nodos:" << endl;
        raiz->recorrer(glm::mat4(1.0f));
        //ent2.draw(model2);

        window.swapBuffers();
        window.pollEvents();
    }

    // Liberar memoria
    delete raiz;
    return 0;
}