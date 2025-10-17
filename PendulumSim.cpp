#include "Pendulum/Pendulum.h"
#include "headers/Vector2D.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Window Setup
const int WIDTH = 800;
const int HEIGHT = 600;
const float PIXELS_PER_METER = 50.0f;

void initOpenGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // dark gray background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WIDTH/2, WIDTH/2, -HEIGHT/2, HEIGHT/2, -1, 1); // center origin
    glMatrixMode(GL_MODELVIEW);
}

int main(){
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Pendulum Simulation", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    initOpenGL();
    
    // Create pendulum: pivot at (0, 4), mass = 1.0kg, radius = 0.3m, rope length = 3.0m
    Pendulum pendulum(Vector2D(0.0f, 4.0f), 1.0f, 0.3f, 3.0f);
    
    const float FIXED_TIMESTEP = 1.0f / 60.0f; // 60 FPS
    float accumulator = 0.0f;
    double lastTime = glfwGetTime();
    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;
        accumulator += deltaTime;
        
        while (accumulator >= FIXED_TIMESTEP) {
            pendulum.applyGravity(Vector2D(0.0f, -9.81f));
            pendulum.update(FIXED_TIMESTEP);
            accumulator -= FIXED_TIMESTEP;
        }
        
        // Render
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        
        pendulum.draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}