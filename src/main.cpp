#include "arbol.cpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Camara.hpp"
#include "Luz.hpp"
#include "Fichero.hpp"
#include "Textura.hpp"
#include "Mesh.hpp"
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

    float fov = glm::radians(45.0f);
    glm::mat4 projection = glm::perspective(fov, 800.0f / 600.0f, 0.1f, 100.0f);

    Fichero fichero("../models/prota.obj");
    Textura textura("../textures/prota.png");
    Mesh mesh(fichero, textura);
    MGMesh ent1(&shader, &mesh);

    Fichero fichero2("../models/cubo.obj");
    Mesh mesh2(fichero2);
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
    cout << "Estructura del árbol:" << endl;
    imprimirArbol(raiz);   

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
        cout << "Recorriendo el árbol y dibujando nodos:" << endl;
        raiz->recorrer(glm::mat4(1.0f));
        //ent2.draw(model2);

        window.swapBuffers();
        window.pollEvents();
    }

    // Liberar memoria
    delete raiz;
    return 0;
}