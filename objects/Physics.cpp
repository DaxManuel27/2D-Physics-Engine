#include "Physics.h"
#include "RectangleCollider.h"
#include <algorithm>
#include <cmath>

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
    for (size_t i = 0; i < bodies.size(); i++) {
        for (size_t j = i + 1; j < bodies.size(); j++) {
            RigidBody* bodyA = bodies[i];
            RigidBody* bodyB = bodies[j];
            
            // Skip if both are static
            if (bodyA->isStaticBody() && bodyB->isStaticBody()) continue;
            
            Collider* colliderA = bodyA->getCollider();
            Collider* colliderB = bodyB->getCollider();
            
            if (!colliderA || !colliderB) continue;
            
            // Handle circle-circle collisions
            if (colliderA->getType() == ColliderType::Circle && 
                colliderB->getType() == ColliderType::Circle) {
                
                Vector2D posA = bodyA->getPosition();
                Vector2D posB = bodyB->getPosition();
                
                float radiusA = colliderA->getRadius();
                float radiusB = colliderB->getRadius();
                
                // Calculate distance between centers
                Vector2D delta = posB - posA;
                float distance = delta.length();
                float minDistance = radiusA + radiusB;
                
                // Check if circles are overlapping
                if (distance < minDistance && distance > 0.0001f) {
                    // Collision detected!
                    
                    // Calculate collision normal (from A to B)
                    Vector2D normal = delta / distance;
                    
                    // Calculate overlap depth
                    float overlap = minDistance - distance;
                    
                    // Separate the circles (push them apart)
                    // If one is static, only move the non-static one
                    if (bodyA->isStaticBody()) {
                        posB = posB + normal * overlap;
                        bodyB->setPosition(posB);
                    } else if (bodyB->isStaticBody()) {
                        posA = posA - normal * overlap;
                        bodyA->setPosition(posA);
                    } else {
                        // Both are dynamic - separate proportionally by mass
                        float totalMass = bodyA->getMass() + bodyB->getMass();
                        float ratioA = bodyB->getMass() / totalMass;
                        float ratioB = bodyA->getMass() / totalMass;
                        
                        posA = posA - normal * (overlap * ratioA);
                        posB = posB + normal * (overlap * ratioB);
                        
                        bodyA->setPosition(posA);
                        bodyB->setPosition(posB);
                    }
                    
                    // Calculate restitution
                    float restitution = std::min(bodyA->getRestitution(), 
                                                bodyB->getRestitution());
                    
                    // Reflect velocity for body A (if not static)
                    if (!bodyA->isStaticBody()) {
                        Vector2D velA = bodyA->getVelocity();
                        float velAlongNormal = velA.dot(normal * -1.0f);
                        
                        if (velAlongNormal < 0) {
                            // Reflect velocity: newVel = vel - 2*(vel·normal)*normal
                            Vector2D newVel = velA + normal * (2.0f * velAlongNormal * (1.0f + restitution) * 0.5f);
                            bodyA->setVelocity(newVel);
                        }
                    }
                    
                    // Reflect velocity for body B (if not static)
                    if (!bodyB->isStaticBody()) {
                        Vector2D velB = bodyB->getVelocity();
                        float velAlongNormal = velB.dot(normal);
                        
                        if (velAlongNormal < 0) {
                            // Reflect velocity: newVel = vel - 2*(vel·normal)*normal
                            Vector2D newVel = velB - normal * (2.0f * velAlongNormal * (1.0f + restitution) * 0.5f);
                            bodyB->setVelocity(newVel);
                        }
                    }
                }
            }
            // Handle circle-rectangle collisions
            else if ((colliderA->getType() == ColliderType::Circle && 
                      colliderB->getType() == ColliderType::Rectangle) ||
                     (colliderA->getType() == ColliderType::Rectangle && 
                      colliderB->getType() == ColliderType::Circle)) {
                
                // Determine which is circle and which is rectangle
                RigidBody* circleBody;
                RigidBody* rectBody;
                Collider* circleCollider;
                Collider* rectCollider;
                
                if (colliderA->getType() == ColliderType::Circle) {
                    circleBody = bodyA;
                    rectBody = bodyB;
                    circleCollider = colliderA;
                    rectCollider = colliderB;
                } else {
                    circleBody = bodyB;
                    rectBody = bodyA;
                    circleCollider = colliderB;
                    rectCollider = colliderA;
                }
                
                Vector2D circlePos = circleBody->getPosition();
                Vector2D rectPos = rectBody->getPosition();
                float radius = circleCollider->getRadius();
                float rectWidth = rectCollider->getWidth();
                float rectHeight = rectCollider->getHeight();
                float rectAngle = rectBody->getAngle();
                
                // Find the closest point on the rectangle to the circle
                float halfWidth = rectWidth / 2.0f;
                float halfHeight = rectHeight / 2.0f;
                
                // Calculate the circle position relative to rectangle
                Vector2D delta = circlePos - rectPos;
                
                // Rotate delta into rectangle's local space (unrotate)
                float cosA = std::cos(-rectAngle);
                float sinA = std::sin(-rectAngle);
                Vector2D localDelta(
                    delta.x * cosA - delta.y * sinA,
                    delta.x * sinA + delta.y * cosA
                );
                
                // Clamp the circle's center to the rectangle bounds in local space
                float closestX = std::max(-halfWidth, std::min(halfWidth, localDelta.x));
                float closestY = std::max(-halfHeight, std::min(halfHeight, localDelta.y));
                
                // Rotate the closest point back to world space
                Vector2D localClosest(closestX, closestY);
                float cosB = std::cos(rectAngle);
                float sinB = std::sin(rectAngle);
                Vector2D closestPoint(
                    localClosest.x * cosB - localClosest.y * sinB + rectPos.x,
                    localClosest.x * sinB + localClosest.y * cosB + rectPos.y
                );
                
                // Calculate distance from circle center to closest point
                Vector2D distVec = circlePos - closestPoint;
                float distance = distVec.length();
                
                // Check for collision
                if (distance < radius) {
                    // Collision detected!
                    
                    // Calculate collision normal (from rect to circle)
                    Vector2D normal;
                    if (distance > 0.0001f) {
                        normal = distVec / distance;
                    } else {
                        // Circle center is at or very close to the closest point
                        // Use a default normal based on which edge we're closest to (in local space)
                        Vector2D localNormal;
                        if (std::abs(localDelta.x) > std::abs(localDelta.y)) {
                            localNormal = Vector2D(localDelta.x > 0 ? 1.0f : -1.0f, 0.0f);
                        } else {
                            localNormal = Vector2D(0.0f, localDelta.y > 0 ? 1.0f : -1.0f);
                        }
                        // Rotate normal back to world space
                        normal = Vector2D(
                            localNormal.x * cosB - localNormal.y * sinB,
                            localNormal.x * sinB + localNormal.y * cosB
                        );
                    }
                    
                    // Calculate overlap depth
                    float overlap = radius - distance;
                    
                    // Separate the objects
                    if (rectBody->isStaticBody()) {
                        // Only move the circle
                        circlePos = circlePos + normal * overlap;
                        circleBody->setPosition(circlePos);
                    } else if (circleBody->isStaticBody()) {
                        // Only move the rectangle
                        rectPos = rectPos - normal * overlap;
                        rectBody->setPosition(rectPos);
                    } else {
                        // Both are dynamic
                        float totalMass = circleBody->getMass() + rectBody->getMass();
                        float circleRatio = rectBody->getMass() / totalMass;
                        float rectRatio = circleBody->getMass() / totalMass;
                        
                        circlePos = circlePos + normal * (overlap * circleRatio);
                        rectPos = rectPos - normal * (overlap * rectRatio);
                        
                        circleBody->setPosition(circlePos);
                        rectBody->setPosition(rectPos);
                    }
                    
                    // Calculate velocity reflection for circle (if not static)
                    if (!circleBody->isStaticBody()) {
                        Vector2D circleVel = circleBody->getVelocity();
                        
                        // Calculate velocity along collision normal
                        float velAlongNormal = circleVel.dot(normal);
                        
                        // Only reflect if moving into the surface
                        if (velAlongNormal < 0) {
                            // Get restitution (bounciness)
                            float restitution = std::min(circleBody->getRestitution(), 
                                                        rectBody->getRestitution());
                            
                            // Reflect velocity along normal: newVel = vel - 2*(vel·normal)*normal
                            // Multiply by restitution to control bounciness
                            Vector2D newVel = circleVel - normal * (2.0f * velAlongNormal * (1.0f + restitution) * 0.5f);
                            circleBody->setVelocity(newVel);
                        }
                    }
                    
                    // Handle dynamic rectangle colliding with static/dynamic circle
                    if (!rectBody->isStaticBody()) {
                        Vector2D rectVel = rectBody->getVelocity();
                        
                        // Calculate velocity along collision normal (reversed for rect)
                        float velAlongNormal = rectVel.dot(normal * -1.0f);
                        
                        if (velAlongNormal < 0) {
                            float restitution = std::min(circleBody->getRestitution(), 
                                                        rectBody->getRestitution());
                            
                            Vector2D newVel = rectVel + normal * (2.0f * velAlongNormal * (1.0f + restitution) * 0.5f);
                            rectBody->setVelocity(newVel);
                        }
                    }
                }
            }
        }
    }
}
