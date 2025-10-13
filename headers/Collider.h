#pragma once
#include "Vector2D.h"

enum class ColliderType{
    Circle,
    Rectangle
};

class Collider {
    protected:
        ColliderType type;
    public:
        Collider(ColliderType t) :type(t) {}
        virtual ~Collider() = default;
        
        ColliderType getType() const { return type;}
        virtual float getRadius() const { return 0.0f; }
        virtual float getWidth() const { return 0.0f; }
        virtual float getHeight() const { return 0.0f; }

};