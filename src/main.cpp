#define GLM_ENABLE_EXPERIMENTAL
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

void test(Shader *shader2d) {

}


void DrawRectangle(float x, float y, float width, float height, glm::vec4 color) {
    shader2D->setVec4("color", color);
    
    float vertices[] = {
        x, y, 0.0f,         // Top-left
        x + width, y, 0.0f, // Top-right
        x + width, y - height, 0.0f, // Bottom-right
        x, y - height, 0.0f // Bottom-left
    };

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void DrawCircle(float x, float y, float radius, glm::vec4 color) {
    shader2D->setVec4("color", color);
    
    const int numSegments = 50;
    float vertices[numSegments * 3];

    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        vertices[i * 3] = x + dx;
        vertices[i * 3 + 1] = y + dy;
        vertices[i * 3 + 2] = 0.0f;
    }

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

int main() {
    Window window(800, 600, "Motor OpenGL 3D");

    Shader shader3D("../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");
    //init3D();
    //Shader shader2d("../shaders/vertex_2d.glsl", "../shaders/fragment_2d.glsl");
    init2D();

    TGestorRecursos rec;

    rec.cargarMateriales();

    rec.ImprimirRecursos();

    float fov = glm::radians(45.0f);
    glm::mat4 projection = glm::perspective(fov, (float) window.width / (float) window.height, 0.1f, 100.0f);
    glm::mat4 orthoProjection = glm::ortho(0.0f, (float) window.width, (float) window.height, 0.0f, -1.0f, 1.0f);

    RecursoMaterial* mat1 = dynamic_cast<RecursoMaterial*>(rec.getRecurso("Cyanrubber"));

    Fichero fichero("../models/prota.obj");
    Textura textura("../textures/prota.png");
<<<<<<< HEAD
    Mesh mesh(fichero,nullopt, mat1->returnMaterial());
    MGMesh ent1(&shader, &mesh);

    Fichero fichero2("../models/cubo.obj");
    //Textura textura2("../textures/ladrillo.png");
    Mesh mesh2(fichero2, textura);
    MGMesh ent2(&shader, &mesh2);
=======
    Mesh mesh(fichero,nullopt, materialObsidiana.GetMaterial());
    MGMesh ent1(&shader3D, &mesh);

    Fichero fichero2("../models/cubo.obj");
    //Textura textura2("../textures/ladrillo.png");
    Mesh mesh2(fichero2, textura, materialJade.GetMaterial());
    MGMesh ent2(&shader3D, &mesh2);
>>>>>>> de90d064c76adb6be83ee95f1e7c1a18d85ae376

    MGEntity ent0(&shader3D);
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
    while (!window.shouldClose()) {
        shader3D.use();
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(window, deltaTime);
        luz.aplicar(shader3D);

        glm::mat4 view = camara.getViewMatrix();
        shader3D.setMat4("view", view);
        shader3D.setMat4("projection", projection);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Recorrer el árbol y dibujar los nodos    
        //cout << "Recorriendo el árbol y dibujando nodos:" << endl;
        raiz->recorrer(glm::mat4(1.0f));
        //ent2.draw(model2);

        glDisable(GL_DEPTH_TEST);

        shader2D->use();
        shader2D->setMat4("projection", orthoProjection);
        
        DrawRectangle(200.0f, 500.0f, 100.0f, 100.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        DrawRectangle(00.0f, 500.0f, 100.0f, 100.0f, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        DrawCircle(200.0f, 200.0f, 50.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);
        shader3D.use();
        shader3D.setMat4("projection", projection);

        std::cout << glm::to_string(orthoProjection) << std::endl;

        window.swapBuffers();
        window.pollEvents();
    }

    // Liberar memoria
    delete raiz;
    return 0;
}