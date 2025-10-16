#include <GLFW/glfw3.h>
#include "headers/RigidBody.h"
#include "headers/CircleCollider.h"
#include "headers/RectangleCollider.h"
#include <iostream>
#include "Physics.h"
#include <vector>
#include <cmath>

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
    
    // Create 3 bouncing balls
    std::vector<RigidBody> balls;
    
    const float ballRadius = 0.5f;  // 0.5 meters radius
    const float ballMass = 1.0f;
    
    // Ball 1 - top left
    RigidBody ball1(Vector2D(-3.0f, 3.0f), ballMass, false);
    ball1.setCollider(new CircleCollider(ballRadius));
    ball1.setRestitution(0.8f);
    ball1.setVelocity(Vector2D(2.0f, -1.0f));  // Initial velocity
    balls.push_back(ball1);
    
    // Ball 2 - top right
    RigidBody ball2(Vector2D(3.0f, 2.0f), ballMass, false);
    ball2.setCollider(new CircleCollider(ballRadius));
    ball2.setRestitution(0.8f);
    ball2.setVelocity(Vector2D(-2.5f, 1.0f));  // Initial velocity
    balls.push_back(ball2);
    
    // Ball 3 - center
    RigidBody ball3(Vector2D(0.0f, 0.0f), ballMass, false);
    ball3.setCollider(new CircleCollider(ballRadius));
    ball3.setRestitution(0.8f);
    ball3.setVelocity(Vector2D(1.0f, 3.0f));  // Initial velocity
    balls.push_back(ball3);
    

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
            // Apply gravity and physics to all balls
            for (auto& ball : balls) {
                physics.applyGravity(ball);
                ball.update(FIXED_TIMESTEP);
                physics.checkWallCollisions(ball);  // Bounce off walls
            }
            
            // Collect all bodies for collision detection
            std::vector<RigidBody*> allBodies;
            for (auto& ball : balls) {
                allBodies.push_back(&ball);
            }
            
            physics.checkBodyCollisions(allBodies);
            
            accumulator -= FIXED_TIMESTEP;
        }

        // Render
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        // Draw all balls
        glColor3f(0.3f, 0.7f, 1.0f);  // Cyan balls
        for (auto& ball : balls) {
            ball.draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

