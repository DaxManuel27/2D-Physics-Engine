#include "RigidBody.h"
#include "Collider.h"
#include "Vector2D.h"
#include <OpenGL/gl.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Initialize static member
float RigidBody::pixelsPerMeter = 50.0f;

RigidBody::RigidBody(const Vector2D& pos, float m, bool stat)
    : position(pos),
      velocity(0, 0),
      acceleration(0, 0),
      mass(m),
      restitution(0.5f),
      friction(0.3f),
      isStatic(stat),
      angle(0.0f),
      angularV(0.0f),
      inertia(m * 0.5f)
{}

Vector2D RigidBody::getPosition() const { return position; }
Vector2D RigidBody::getVelocity() const { return velocity; }
Vector2D RigidBody::getAcceleration() const { return acceleration; }
float RigidBody::getMass() const { return mass; }
float RigidBody::getRestitution() const { return restitution; }
float RigidBody::getFriction() const { return friction; }
float RigidBody::getAngle() const { return angle; }
bool RigidBody::isStaticBody() const { return isStatic; }
Collider* RigidBody::getCollider() const { return collider; }

void RigidBody::setPosition(const Vector2D& pos) { position = pos; }
void RigidBody::setVelocity(const Vector2D& vel) { velocity = vel; }
void RigidBody::setAcceleration(const Vector2D& acc) { acceleration = acc; }
void RigidBody::setRestitution(float r) { restitution = r; }
void RigidBody::setFriction(float f) { friction = f; }
void RigidBody::setAngle(float a) { angle = a; }
void RigidBody::setCollider(Collider* c) { collider = c; }

void RigidBody::applyForce(const Vector2D& force) {
    if (isStatic) return;
    // F = ma, so a = F/m
    float invMass = (mass > 0) ? (1.0f / mass) : 0.0f;
    acceleration += force * invMass;
}

void RigidBody::draw() const {
    if(!collider) return;
    
    // Convert position from meters to pixels
    float x = position.x * pixelsPerMeter;
    float y = position.y * pixelsPerMeter;
    
    glPushMatrix(); // Save the current transform
    glTranslatef(x, y, 0.0f); // Move to body position
    glRotatef(angle * (180.0f / M_PI), 0.0f, 0.0f, 1.0f); // Rotate body if needed

    // Pick color based on static/dynamic
    if (isStatic)
        glColor3f(0.3f, 0.3f, 0.3f);  // Gray for static
    else
        glColor3f(0.2f, 0.7f, 1.0f);  // Cyan for dynamic

    // Detect collider type
    switch (collider->getType()) {
        case ColliderType::Circle: {
            // Scale radius from meters to pixels
            float radiusPixels = collider->getRadius() * pixelsPerMeter;
            int segments = 32;
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.0f, 0.0f);
            for (int i = 0; i <= segments; i++) {
                float angle = i * 2.0f * M_PI / segments;
                float cx = cos(angle) * radiusPixels;
                float cy = sin(angle) * radiusPixels;
                glVertex2f(cx, cy);
            }
            glEnd();
            break;
        }

        case ColliderType::Rectangle: {
            // Scale dimensions from meters to pixels
            float w = collider->getWidth() * pixelsPerMeter;
            float h = collider->getHeight() * pixelsPerMeter;
            glBegin(GL_QUADS);
            glVertex2f(-w / 2, -h / 2);
            glVertex2f( w / 2, -h / 2);
            glVertex2f( w / 2,  h / 2);
            glVertex2f(-w / 2,  h / 2);
            glEnd();
            break;
        }

        default:
            // fallback (point)
            glPointSize(6.0f);
            glBegin(GL_POINTS);
            glVertex2f(0.0f, 0.0f);
            glEnd();
            break;
    }

    glPopMatrix(); // Restore transform
}
void RigidBody::update(float dt){
    if(isStatic) return;
    velocity += acceleration * dt;
    position += velocity * dt;
    angle += angularV * dt;
    acceleration = Vector2D(0,0);

}