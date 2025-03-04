#ifndef TEGE_WINDOW_HPP
#define TEGE_WINDOW_HPP

#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
    public:
        int width, height;
        Window(int width, int height, const char* title) {
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
        ~Window() {
            glfwDestroyWindow(window);
            glfwTerminate();
        }

        bool shouldClose() const {return glfwWindowShouldClose(window);}
        void swapBuffers() {glfwSwapBuffers(window);}
        void pollEvents() {glfwPollEvents();}
        GLFWwindow* getNativeWindow() const {return window;}

    private:
        GLFWwindow* window;

        static void ajustar_size_ventana(GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);

            // Obtener la instancia de Window y actualizar la proyección ortográfica
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (win) {
                win->width = width;
                win->height = height;
            }
        }

    };

#endif