#include <GLFW/glfw3.h>
#include "headers/RigidBody.h"
#include "headers/CircleCollider.h"
#include <iostream>
#include "Physics.h"

// ------------------ Window Setup ------------------
const int WIDTH = 800;
const int HEIGHT = 600;
const float PIXELS_PER_METER = 50.0f;  // Scale factor: 50 pixels = 1 meter

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

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "2D Physics Engine - Real Gravity (9.8 m/s²)", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    initOpenGL();

    // ------------------ Physics setup ------------------

    RigidBody::pixelsPerMeter = PIXELS_PER_METER;
    

    float worldWidthMeters = WIDTH / PIXELS_PER_METER;   // 16 meters
    float worldHeightMeters = HEIGHT / PIXELS_PER_METER; // 12 meters
    
    Physics physics(worldWidthMeters, worldHeightMeters);
    

    RigidBody ball(Vector2D(0.0f, 5.0f), 1.0f, false);  // Start 5 meters high
    CircleCollider circle(1.0f);  // 1 meter radius (will render as 50 pixels)
    ball.setVelocity(Vector2D(3.0f, 2.0f));  // 3 m/s horizontal, 2 m/s upward
    ball.setRestitution(0.8f);  // Bouncy!
    ball.setCollider(&circle);
    

    const float FIXED_TIMESTEP = 1.0f / 60.0f; // 60 FPS
    float accumulator = 0.0f;
    double lastTime = glfwGetTime();
    
    // ------------------ Main loop ------------------
    while (!glfwWindowShouldClose(window)) {

        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;
        accumulator += deltaTime;
        

        while (accumulator >= FIXED_TIMESTEP) {
            // Apply gravity (9.8 m/s²)
            physics.applyGravity(ball);
            
            // Update physics
            ball.update(FIXED_TIMESTEP);
            
            // Check wall collisions
            physics.checkWallCollisions(ball);
            
            accumulator -= FIXED_TIMESTEP;
        }

        // Render
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        ball.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
