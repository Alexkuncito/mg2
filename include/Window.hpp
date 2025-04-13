#ifndef TEGE_WINDOW_HPP
#define TEGE_WINDOW_HPP

#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
public:
    int width, height;
    Window(int width, int height, const char* title);
    ~Window();

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    GLFWwindow* getNativeWindow() const;
    void close();

private:
    GLFWwindow* window;

    static void ajustar_size_ventana(GLFWwindow* window, int width, int height);
};

#endif
