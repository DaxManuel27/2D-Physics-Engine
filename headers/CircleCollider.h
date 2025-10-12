#pragma once
#include "Collider.h"

class CircleCollider : public Collider {
private:
    float radius;

public:
    CircleCollider(float r) 
        : Collider(ColliderType::Circle), radius(r) {}

    float getRadius() const override { return radius; }
};
