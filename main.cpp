#include <GLFW/glfw3.h>
#include "headers/RigidBody.h"
#include "headers/CircleCollider.h"
#include "headers/RectangleCollider.h"
#include <iostream>
#include "Physics.h"
#include <vector>
#include <cmath>
#include <ctime>

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
    
    // ------------------ Create Cup Structure ------------------
    std::vector<RigidBody> staticObjects;
    
    // Cup position and dimensions
    float cupBottomY = -4.0f;
    float cupX = 2.0f;
    float cupWidth = 3.0f;
    float cupWallHeight = 2.5f;
    float wallThickness = 0.2f;
    
    // Cup bottom
    RigidBody cupBottom(Vector2D(cupX, cupBottomY), 1.0f, true);
    cupBottom.setCollider(new RectangleCollider(cupWidth, wallThickness));
    cupBottom.setRestitution(0.3f);
    staticObjects.push_back(cupBottom);
    
    // Cup left wall
    RigidBody cupLeftWall(Vector2D(cupX - cupWidth/2 + wallThickness/2, cupBottomY + cupWallHeight/2), 1.0f, true);
    cupLeftWall.setCollider(new RectangleCollider(wallThickness, cupWallHeight));
    cupLeftWall.setRestitution(0.3f);
    staticObjects.push_back(cupLeftWall);
    
    // Cup right wall
    RigidBody cupRightWall(Vector2D(cupX + cupWidth/2 - wallThickness/2, cupBottomY + cupWallHeight/2), 1.0f, true);
    cupRightWall.setCollider(new RectangleCollider(wallThickness, cupWallHeight));
    cupRightWall.setRestitution(0.3f);
    staticObjects.push_back(cupRightWall);
    
    // ------------------ Create Ramp ------------------
    float rampLength = 8.0f;
    float rampWidth = 0.3f;
    float rampAngle = -0.4f;  // Radians (about -23 degrees)
    
    // Position ramp to lead into the cup
    float rampX = -4.0f;
    float rampY = 0.0f;
    
    RigidBody ramp(Vector2D(rampX, rampY),1.0f, true);
    ramp.setCollider(new RectangleCollider(rampLength, rampWidth));
    ramp.setAngle(rampAngle);
    ramp.setRestitution(0.4f);
    staticObjects.push_back(ramp);
    
    // ------------------ Create Balls ------------------
    std::vector<RigidBody> balls;
    
    const float ballRadius = 0.02f;
    const float ballMass = 0.001f;  // Small consistent mass
    int numBalls = 50;  // Start with 50 to test (1000 might be slow!)
    float startX = -6.5f;
    float startY = 1.5f;
    float rangeX = 1.0f;  // Spread 1 meter horizontally
    float rangeY = 1.0f;  // Spread 1 meter vertically
    
    // Seed the random number generator
    srand(time(NULL));
    
    for(int i = 0; i < numBalls; i++) {
        // Generate random offsets (rand() / (float)RAND_MAX gives 0.0 to 1.0)
        float randomX = startX + rangeX * (rand() / (float)RAND_MAX);
        float randomY = startY + rangeY * (rand() / (float)RAND_MAX);
        
        RigidBody ball(Vector2D(randomX, randomY), ballMass, false);
        ball.setCollider(new CircleCollider(ballRadius));
        ball.setRestitution(0.6f);
        balls.push_back(ball);
    }
    
    // NOW start the game loop (OUTSIDE the ball creation loop)
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
            
            // Collect all bodies for collision detection (balls + static objects)
            std::vector<RigidBody*> allBodies;
            for (auto& ball : balls) {
                allBodies.push_back(&ball);
            }
            for (auto& obj : staticObjects) {
                allBodies.push_back(&obj);
            }
            
            physics.checkBodyCollisions(allBodies);
            
            accumulator -= FIXED_TIMESTEP;
        }

        // Render
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        // Draw static objects (cup and ramp)
        for (auto& obj : staticObjects) {
            obj.draw();
        }

        // Draw all balls
        for (auto& ball : balls) {
            ball.draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

