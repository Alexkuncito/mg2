#include "arbol.cpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Camara.hpp"


Camera camera(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 5.0f, 50.0f);

void processInput(Window& window, float deltaTime) {
    GLFWwindow* glfwWindow = window.getNativeWindow();

    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveForward(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveBackward(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveLeft(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveRight(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_UP) == GLFW_PRESS)
        camera.zoomIn(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.zoomOut(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.rotateLeft(deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.rotateRight(deltaTime);
}


int main() {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    Window window(800, 600, "Motor OpenGL 3D");

    Shader shader("../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Creación de nodos
    Nodo* raiz = new Nodo(0);
    Nodo* hijo1 = new Nodo(1);
    Nodo* hijo2 = new Nodo(2);
    Nodo* hijo3 = new Nodo(3);
    Nodo* hijo4 = new Nodo(4);
    Nodo* hijo5 = new Nodo(5);
    Nodo* hijo6 = new Nodo(6);

    // Construcción de la jerarquía del árbol
    raiz->agregarHijo(hijo1);
    raiz->agregarHijo(hijo2);
    hijo2->agregarHijo(hijo3);
    hijo3->agregarHijo(hijo6);
    hijo2->agregarHijo(hijo4);
    raiz->agregarHijo(hijo5);

    //Aplicar transformaciones
    //raiz->setRotacion(glm::vec3(0.0f, 45.0f, 0.0f));
    //hijo1->setEscalado(glm::vec3(1.5f, 1.5f, 1.5f));
    hijo2->setTraslacion(glm::vec3(30.0f, 20.0f, 10.0f));
    hijo3->setEscalado(glm::vec3(0.5f, 0.5f, 0.5f));
    hijo4->setRotacion(glm::vec3(0.0f, 0.0f, 90.0f));
    //hijo5->setTraslacion(glm::vec3(2.0f, 2.0f, 2.0f));
    hijo6->setRotacion(glm::vec3(0.0f, 180.0f, 0.0f));
    
    // Imprimir estructura del árbol
    cout << "Estructura del árbol:" << endl;
    imprimirArbol(raiz);

    // Recorrer el árbol y dibujar los nodos
    cout << "Recorriendo el árbol y dibujando nodos:" << endl;
    raiz->recorrer(glm::mat4(1.0f));

    glEnable(GL_DEPTH_TEST);

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    float lastTime = glfwGetTime();
    while (!window.shouldClose()) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(window, deltaTime);

        glm::mat4 view = camera.getViewMatrix();
        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glClearColor(51 / 255.0f, 77 / 255.0f, 77 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        window.swapBuffers();
        window.pollEvents();
    }

    // Liberar memoria
    delete raiz;
    return 0;
}