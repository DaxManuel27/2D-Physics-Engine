#include "Physics.h"

Physics::Physics(float width, float height, const Vector2D& grav) 
    : worldWidth(static_cast<int>(width)), worldHeight(static_cast<int>(height)), gravity(grav) {}

void Physics::applyGravity(RigidBody& body){
    if(!body.isStaticBody()){
        body.applyForce(gravity * body.getMass());
    }
}

void Physics::checkWallCollisions(RigidBody& body) {
    Collider* collider = body.getCollider();
    if (!collider) return;

    Vector2D pos = body.getPosition();
    Vector2D vel = body.getVelocity();
    bool collided = false;

    if (collider->getType() == ColliderType::Circle) {
        float radius = collider->getRadius();
        
        // Left wall
        if (pos.x - radius < -worldWidth / 2) {
            pos.x = -worldWidth / 2 + radius;
            vel.x = -vel.x * body.getRestitution();
            collided = true;
        }
        // Right wall
        if (pos.x + radius > worldWidth / 2) {
            pos.x = worldWidth / 2 - radius;
            vel.x = -vel.x * body.getRestitution();
            collided = true;
        }
        // Bottom wall
        if (pos.y - radius < -worldHeight / 2) {
            pos.y = -worldHeight / 2 + radius;
            vel.y = -vel.y * body.getRestitution();
            collided = true;
        }
        // Top wall
        if (pos.y + radius > worldHeight / 2) {
            pos.y = worldHeight / 2 - radius;
            vel.y = -vel.y * body.getRestitution();
            collided = true;
        }
    }
    
    if (collided) {
        body.setPosition(pos);
        body.setVelocity(vel);
    }
}

void Physics::checkBodyCollisions(std::vector<RigidBody*>& bodies) {
    // TODO: Implement body-to-body collision detection
    (void)bodies; // Suppress unused parameter warning
}
