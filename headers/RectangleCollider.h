#pragma once
#include "Collider.h"

class RectangleCollider : public Collider {
private:
    float width;
    float height;

public:
    RectangleCollider(float w, float h)
        : Collider(ColliderType::Rectangle), width(w), height(h) {}

    float getWidth() const override { return width; }
    float getHeight() const override { return height; }
};
