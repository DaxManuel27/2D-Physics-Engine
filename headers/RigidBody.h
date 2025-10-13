#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Vector2D.h"

class Collider; // Forward declaration

class RigidBody{
    protected:
        Vector2D position;
        Vector2D velocity;
        Vector2D acceleration;
        float mass;
        float restitution;
        float friction;
        bool isStatic;

        float angle;
        float angularV;
        float inertia;
        
        Collider* collider = nullptr;
    
    public:
        static float pixelsPerMeter;  // Scale factor for rendering
        RigidBody(const Vector2D& pos, float m, bool stat = false);
        virtual ~RigidBody() = default;
        Vector2D getPosition() const;
        Vector2D getVelocity() const;
        Vector2D getAcceleration() const;

        float getMass() const;
        float getRestitution() const;
        float getFriction() const;
        float getAngle() const;
        bool isStaticBody() const;
        Collider* getCollider() const;
        
        void setPosition(const Vector2D& pos);
        void setVelocity(const Vector2D& vel);
        void setAcceleration(const Vector2D& acc);
        void setRestitution(float r);
        void setFriction(float f);
        void setAngle(float a);
        void setCollider(Collider* c);
        void update(float dt);
        void applyForce(const Vector2D& force);
        virtual void draw() const;

};

#endif