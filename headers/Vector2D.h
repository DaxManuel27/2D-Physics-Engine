#ifndef VECTOR2D_H
#define VECTOR2D_H

class Vector2D {
public:
    float x;
    float y;

    // Constructors
    Vector2D();
    Vector2D(float x, float y);

    // Operator overloads
    Vector2D operator+(const Vector2D& v) const;
    Vector2D operator-(const Vector2D& v) const;
    Vector2D operator*(float scalar) const;
    Vector2D operator/(float scalar) const;
    Vector2D& operator+=(const Vector2D& v);
    Vector2D& operator-=(const Vector2D& v);
    Vector2D& operator*=(float s);

    // Vector operations
    float length();
    Vector2D normalize();
    float dot(const Vector2D& v) const;
    Vector2D perpendicular() const;
    
    // Static methods
    static float distance(const Vector2D& a, const Vector2D& b);
};

#endif