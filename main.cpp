#include <GLFW/glfw3.h>
#include "headers/RigidBody.h"
#include "headers/CircleCollider.h"
#include <iostream>

// ------------------ Window Setup ------------------
const int WIDTH = 800;
const int HEIGHT = 600;

void initOpenGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // dark gray background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WIDTH/2, WIDTH/2, -HEIGHT/2, HEIGHT/2, -1, 1); // center origin
    glMatrixMode(GL_MODELVIEW);
}

// ------------------ Main ------------------
int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "RigidBody OpenGL Test", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    initOpenGL();

    // ------------------ Physics setup ------------------
    RigidBody ball(Vector2D(0.0f, 0.0f), 1.0f, false);
    CircleCollider circle(50.0f); // radius 50 pixels
    ball.setCollider(&circle);

    // ------------------ Main loop ------------------
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glLoadIdentity();

        // Draw the RigidBody (calls your OpenGL draw code)
        ball.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
