#include "Pendulum.h"
#include <OpenGL/gl.h>
#include <cmath>
#define M_PI 3.14159265358979323846

Pendulum::Pendulum(const Vector2D& pos, float m, float r, float ropeLength)
    : mass(pos, m, false),
      massCollider(ColliderType::Circle),
      ropeLength(ropeLength),
      angle(M_PI / 4.0f), // Start at 45 degrees
      angularV(0.0f),
      angularA(0.0f),
      point(pos),
      position(pos),
      velocity(0.0f, 0.0f),
      acceleration(0.0f, 0.0f),
      isStatic(false)
{
    // Set initial position of mass based on angle and rope length
    float initialX = point.x + ropeLength * sin(angle);
    float initialY = point.y - ropeLength * cos(angle);
    mass.setPosition(Vector2D(initialX, initialY));
}

void Pendulum::update(float dt){
    if(isStatic) return;
    
    // Update the rigidbody (this applies gravity and other forces)
    mass.update(dt);
    
    // Get current mass position
    Vector2D massPos = mass.getPosition();
    
    // Apply pendulum constraint: keep mass at fixed distance from pivot
    Vector2D toMass = massPos - point;
    float currentLength = sqrt(toMass.x * toMass.x + toMass.y * toMass.y);
    
    // Normalize and scale to rope length
    if (currentLength > 0.001f) {
        toMass = toMass * (ropeLength / currentLength);
        Vector2D constrainedPos = point + toMass;
        
        // Update angle based on position
        angle = atan2(toMass.x, -toMass.y);
        
        // Calculate velocity perpendicular to rope (tangential velocity)
        Vector2D vel = mass.getVelocity();
        Vector2D radialDir = toMass * (1.0f / ropeLength);
        float radialVel = vel.x * radialDir.x + vel.y * radialDir.y;
        Vector2D tangentialVel = vel - radialDir * radialVel;
        
        // Set constrained position and tangential velocity
        mass.setPosition(constrainedPos);
        mass.setVelocity(tangentialVel);
    }
}

void Pendulum::draw() const {
    // Get the mass position from rigidbody
    Vector2D massPos = mass.getPosition();
    
    // Draw the rope (line from pivot point to mass)
    glColor3f(0.8f, 0.8f, 0.8f); // Light gray for rope
    glLineWidth(4.0f);
    glBegin(GL_LINES);
    glVertex2f(point.x * 50.0f, point.y * 50.0f);  // Pivot point
    glVertex2f(massPos.x * 50.0f, massPos.y * 50.0f); // Mass position
    glEnd();
    
    // Draw the pivot point
    glColor3f(1.0f, 0.0f, 0.0f); // Red for pivot
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glVertex2f(point.x * 50.0f, point.y * 50.0f);
    glEnd();
    
    // Draw the mass (circle)
    glColor3f(0.2f, 0.7f, 1.0f); // Cyan for mass
    glPushMatrix();
    glTranslatef(massPos.x * 50.0f, massPos.y * 50.0f, 0.0f);
    
    // Draw circle for mass
    const int segments = 30;
    const float massRadius = 0.3f; // 0.3 meters
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        float x = massRadius * 50.0f * cos(angle);
        float y = massRadius * 50.0f * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
    
    glPopMatrix();
}

void Pendulum::applyForce(const Vector2D& force) {  
    mass.applyForce(force);
}

void Pendulum::applyTorque(const Vector2D& torque) {
    // Calculate torque effect on angular acceleration
    // torque = I * alpha, where I is moment of inertia
    // For a point mass: I = m * r^2
    float I = mass.getMass() * ropeLength * ropeLength;
    if (I > 0) {
        // Assuming torque.x represents the magnitude of torque
        angularA = torque.x / I;
    }
}

void Pendulum::applyGravity(const Vector2D& gravity) {
    // Apply gravity force: F = m * g
    Vector2D gravityForce = gravity * mass.getMass();
    mass.applyForce(gravityForce);
}

