#ifndef HORUSEYE_FRAMEWORK_VECTOR2D_H_
#define HORUSEYE_FRAMEWORK_VECTOR2D_H_

namespace ugdk {

// 2 dimension vectors, using floats.
class Vector2D {
  public:
    // Initializes both fields with 0.0f
    Vector2D() : x(0.0f), y(0.0f) {}

    // Initializes both fields with val.
    Vector2D(float val) : x(val), y(val) {}

    // Initializes both fields with given values.
    Vector2D(float x, float y) : x(x), y(y) {}

    ~Vector2D() { }

    // Fields are accessible as x/y or val[0] and val[1].
    union {
        struct { float x, y;   };
        struct { float val[2]; };
    };

    // Returns the norm-1.
    float NormOne() const;

    // Returns the norm-2.
    float Length() const;
    
    // Returns the norm-2 squared.
    float LengthSquared() const { return (x*x) + (y*y); }

    // Returns the angle of this vector.
    float Angle() const;

    // Returns a new vector equivalent to this vector normalized.
    Vector2D Normalize() const;

    // Returns a new vector equivalent to this vector rotated "angle" counter-clockwise.
    Vector2D Rotate(const float angle) const;

    // Static methods
    static Vector2D Add(const Vector2D &a, const Vector2D &b) {
        return Vector2D(a.x + b.x, a.y + b.y);
    }
    static Vector2D Subtract(const Vector2D &a, const Vector2D &b) {
        return Vector2D(a.x - b.x, a.y - b.y);
    }
    static Vector2D Multiply(const Vector2D &a, const float &scalar) {
        return Vector2D(a.x * scalar, a.y * scalar);
    }
    static float InnerProduct(const Vector2D &a, const Vector2D &b) {
        return (a.x * b.x) + (a.y * b.y);
    }

    // Deprecated. Use the upper case method.
    float length() const { return Length(); }
	
	// Deprecated. Use the upper case method.
	float angle() const { return Angle(); }

    // Deprecated. Use the method function Normalize.
    static Vector2D Normalized(Vector2D &a) { return a.Normalize(); }

    // Deprecated. Use the method function Rotate.
    static Vector2D Rotate(Vector2D &a, float angle) {
        return a.Rotate(angle);
    }
};

Vector2D operator+(const Vector2D &left, const Vector2D &right);
Vector2D operator-(const Vector2D &left, const Vector2D &right);
Vector2D operator*(const float  &scalar, const Vector2D &right);
Vector2D operator*(const Vector2D &left, const float   &scalar);
Vector2D operator/(const Vector2D &left, const float   &scalar);
float    operator*(const Vector2D &left, const Vector2D &right);

}  // namespace framework

#endif  // HORUSEYE_FRAMEWORK_VECTOR2D_H_
