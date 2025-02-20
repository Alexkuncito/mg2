#ifndef TEGE_WINDOW_HPP
#define TEGE_WINDOW_HPP

#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
    public:
        Window(int width, int height, const char* title) {
            if (!glfwInit()) {
                std::cerr << "Error inicializando GLFW\n";
                exit(EXIT_FAILURE);
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(width, height, title, nullptr, nullptr);
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

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    };

#endif