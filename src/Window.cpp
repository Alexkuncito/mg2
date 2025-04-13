#include "Window.hpp"

Window::Window(int width, int height, const char* title) {
    if (!glfwInit()) {
        std::cerr << "Error inicializando GLFW\n";
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    this->width = width;
    this->height = height;
    if (!window) {
        std::cerr << "Error al crear la ventana\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    // Inicialización de GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error inicializando GLAD\n";
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, ajustar_size_ventana);
    glClearColor(51 / 255.0f, 77 / 255.0f, 77 / 255.0f, 1.0f);
    glfwSetWindowUserPointer(window, this);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

GLFWwindow* Window::getNativeWindow() const {
    return window;
}

void Window::close() {
    if (window) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void Window::ajustar_size_ventana(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    // Obtener la instancia de Window y actualizar la proyección ortográfica
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->width = width;
        win->height = height;
    }
}
