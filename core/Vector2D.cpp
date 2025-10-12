#include "Vector2D.h"
#include <cmath>

// Constructors
Vector2D::Vector2D() : x(0), y(0) {}

Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

// Operator overloads
Vector2D Vector2D::operator+(const Vector2D& v) const {
    return Vector2D(x + v.x, y + v.y);
}

Vector2D Vector2D::operator-(const Vector2D& v) const {
    return Vector2D(x - v.x, y - v.y);
}

Vector2D Vector2D::operator*(float scalar) const {
    return Vector2D(x * scalar, y * scalar);
}

Vector2D Vector2D::operator/(float scalar) const {
    return Vector2D(x / scalar, y / scalar);
}

Vector2D& Vector2D::operator+=(const Vector2D& v) {
    x += v.x;
    y += v.y;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& v) {
    x -= v.x;
    y -= v.y;
    return *this;
}

Vector2D& Vector2D::operator*=(float s) {
    x *= s;
    y *= s;
    return *this;
}

// Vector operations
float Vector2D::length() {
    return std::sqrt(x * x + y * y);
}

Vector2D Vector2D::normalize() {
    float len = length();
    return Vector2D(x / len, y / len);
}

float Vector2D::dot(const Vector2D& v) const {
    return (x * v.x + y * v.y);
}

Vector2D Vector2D::perpendicular() const {
    return Vector2D(-y, x);
}

// Static methods
float Vector2D::distance(const Vector2D& a, const Vector2D& b) {
    return (a - b).length();
}
