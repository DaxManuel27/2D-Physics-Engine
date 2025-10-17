#ifndef PENDULUM_H
#define PENDULUM_H
#include "../headers/RigidBody.h"
#include "../headers/CircleCollider.h"

class Pendulum{
    protected: 
        RigidBody mass;
        Collider massCollider;
        float ropeLength;
        float angle;
        float angularV;
        float angularA;
        Vector2D point;
        Vector2D position;
        Vector2D velocity;
        Vector2D acceleration;
        bool isStatic;
        
    public:
        Pendulum(const Vector2D& pos, float m, float r, float ropeLength);
        ~Pendulum() = default;
        void update(float dt);
        void draw() const;
        void applyForce(const Vector2D& force);
        void applyTorque(const Vector2D& torque);
        void applyGravity(const Vector2D& gravity);
};

#endif