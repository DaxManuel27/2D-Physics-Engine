#ifndef PHYSICS_H
#define PHYSICS_H
#include "RigidBody.h"
#include "CircleCollider.h"
#include <vector>

class Physics {
    private: 
        int worldWidth;
        int worldHeight;
        Vector2D gravity;

    public:
        Physics(float width, float height, const Vector2D& grav = Vector2D(0, -9.8f));
        void checkWallCollisions(RigidBody& body);
        void checkBodyCollisions(std::vector<RigidBody*>& bodies);
        void applyGravity(RigidBody& body);
        
};

#endif