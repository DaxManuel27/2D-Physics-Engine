#ifndef PENDULUM_H
#define PENDULUM_H
#include "RigidBody.h"

class Pendulum{
    protected: 
        RigidBody mass;
        CircleCollider massCollider;
        float ropeLength;
        float angle;
        float anglarV;
        float angularA;
        Vector2D point;
        
    public:
        Pendulum(const Vector2D& pos, float m, float r, float ropeLength);
        ~Pendulum() = default;
        void update(float dt);
        void draw() const;
        
}