
#ifndef UGDK_MATH_VECTOR2D_H_
#define UGDK_MATH_VECTOR2D_H_

#include <ugdk/structure/types.h>
#include <ugdk/math.h>

#include <glm/vec2.hpp>

#include <array>
#include <tuple>

namespace ugdk {
namespace math {

// 2 dimension vectors, using doubles.
class Vector2D {
  public:
    /// Initializes both fields with 0.0
    /** Create the vector (x,y) with x = 0 and y =0
    */
    Vector2D() : x(0.0), y(0.0) {}

    /// Initializes both fields with val.
    /** Create the vector (x,y) with x = val and y = val being val a constant
    *  @param val is a constant
    */
    explicit Vector2D(double value) : x(value), y(value) {}

    /// Initializes both fields with given values.
    /** Create the vector (x,y) with x = x-value and y = y-value
    *  @param x is the x-value argument
    *  @param y is the y-value argument
    */
    Vector2D(double _x, double _y) : x(_x), y(_y) {}

    /// Copy constructor from Integer2D.
    explicit Vector2D(const math::Integer2D& int2d);

    /// Copy constructor from glm::vec2
    Vector2D(const glm::vec2 rhs) : x(rhs.x), y(rhs.y) {}

    ~Vector2D() { }

    // Fields are accessible as x/y or val[0] and val[1].
    union {
        struct { double x, y;   };
        struct { double val[2]; };
    };

    double get_x() const { return x; }
    double get_y() const { return y; }
    void set_x(double x_) { x = x_; }
    void set_y(double y_) { y = y_; }


    /// Returns the norm-1 of this vector.
    /** The norm-1 of a vector (x,y) is |x| + |y|.
    *  @return Norm-1
    *  @see Length()
    */
    double NormOne() const;

    /// Returns the norm-2 of this vector
    /** The norm-2 of a vector (x,y) is sqrt( x^2 + y^2 )
    *  @return Norm-2
    *  @see NormOne()
    *  @see LenghtSquared()
    */
    double Length() const;
    
    /// Returns the norm-2 squared
    /** The norm-2 squared of a vector (x,y) is (x^2 + y^2)
    *  @return Norm-2 Squared
    *  @see NormOne()
    *  @see Lenght()
    */
    double LengthSquared() const { return (x*x) + (y*y); }

    /// Returns the angle (in radians) of this vector
    /** Return the angle (in radians) associated to this vector
    *  @return Angle
    */
    double Angle() const;

    /// Returns a new vector equal to this vector normalized
    /** The normalized vector (x,y) is (x,y) / Lenght(x,y)
    *  @return Normalized Vector
    *  @see Lenght()
    */
    Vector2D Normalize() const;

    /// Returns a new vector equal to this vector rotated by "angle" (in radians) counter-clockwise.
    /** The vector rotated of a vector (x,y) is (x * cos(angle) - y * sin(angle) , x * sin(angle) + y * cos(angle));  angle in radians
    *  @return Rotated Vector
    *  @see Angle
    */
    Vector2D Rotate(double angle) const;
    
    /// Mirrors this Vector2D (in-place) by the "axis" axis. Include types.h and use "using namespace ugdk::enums".
    /** @param axis Either mirroraxis::HORZ, mirroraxis::VERT, mirroraxis::DIAG_UP, or mirroraxis::DIAG_DOWN.
     *  @see Mirrored
     */
    void Mirror(const ugdk::enums::mirroraxis::MirrorAxis axis);
    
    /// Returns a new Vector2D, mirrored by the "axis" axis. Include types.h and use "using namespace ugdk::enums".
    /** @param axis Either mirroraxis::HORZ, mirroraxis::VERT, mirroraxis::DIAG_UP, or mirroraxis::DIAG_DOWN.
     *  @see Mirror
     */
    Vector2D Mirrored(const ugdk::enums::mirroraxis::MirrorAxis axis) const;

    /// Returns a new vector which is this vector scaled coordinate by coordinate with "scale".
    /** The resulting scaled vector is (this->x * scale.x, this->y * scale.y).
     *  @return scaled vector
     */
    Vector2D Scale(const Vector2D& scale) const;

    /// In-place integer division, coordinate by coordinate.
    /** @see Divided
    *  @see Scale
    */
    void Divide(const Vector2D& divider);

    /// Returns a new Integer2D equal to this divided coordinate by coordinate by "multiplier".
    /** @see Divide
    *  @see Scale
    */
    Vector2D Divided(const Vector2D& divider) const;

    // Static methods

    /// Static method that returns a vector equal to the sum of two others
    /** The sum of two vectors (x,y) (w,z) is igual to the vector (x+w, y+z)
    *  @return Sum Vector
    */
    static Vector2D Add(const Vector2D& a, const Vector2D& b) {
        return Vector2D(a.x + b.x, a.y + b.y);
    }
    
    /// Static method that returns a vector equal to the subtraction of two others
    /** The subtraction of two vectors (x,y) (z,w) is equal to the vector (x-z,y-w) in this order
    *  @return Subtract Vector 
    */
    static Vector2D Subtract(const Vector2D& a, const Vector2D& b) {
        return Vector2D(a.x - b.x, a.y - b.y);
    }
    
    /// Static method that returns a vector equal to the a vector multiplied by a scalar
    /** A vector (x,y) multiplied by a scalar a is equal to the vector (x * scalar, y*scalar)
    *  @return Scalar Multiplied Vector
    */
    static Vector2D Multiply(const Vector2D& a, double scalar) {
        return Vector2D(a.x * scalar, a.y * scalar);
    }
    
    /// Static method that returns a scalar equal to the inner product of two vectors
    /** By definition the inner product of two vectors (x,y) (w,z) is equal to (x*w) + (y*z)
    *  @return InnerProduct
    */
    static double InnerProduct(const Vector2D& a, const Vector2D& b) {
        return (a.x * b.x) + (a.y * b.y);
    }

    /// Deprecated. Use the upper case method.
    /**Deprecated method that returns the Lenght of a vector (x,y). Use instead Lenght method
    *  @return lenght
    *  @see Lenght()
    */
    double length() const { return Length(); }
	
	/// Deprecated. Use the upper case method.
    /** Depprecated method that returns the angle associated to a vector (x,y). Use instead Angle()
    *  @return angle
    *  @see Angle()
    */
	double angle() const { return Angle(); }

    /// Deprecated. Use the method function Normalize.
    /** Deprecated method that returns a normalized vector of a vector (x,y). Use instead Normalize
    *  @return normalized vector
    *  @see Normalize()
    */
    static Vector2D Normalized(Vector2D& a) { return a.Normalize(); }

    /// Deprecated. Use the method function Rotate.
    /** Deprecated method that returns a rotated vector of a vector (x,y). Use instead Rotate
    *  @return rotated vector
    *  @see Rotate
    */
    static Vector2D Rotate(Vector2D& a, double angle) {
        return a.Rotate(angle);
    }


	/// Compares this vector to the given vector, return True if they are equivalent.
		/** Two vectors are equivalent if the coordinates are equal.
        *  @return True if vectors are equivalent.
        */
	bool operator==(const Vector2D& rhs) {
		return x==rhs.x && y==rhs.y;
	}

    // TODO document and revise
    Vector2D& operator+=(const Vector2D &other);
    
    // TODO document and revise
    Vector2D& operator-=(const Vector2D &other);
    
    // TODO document
    Vector2D& operator*=(double other);
    
    /// Method that returns a vector equal to the sum of two others
        /** The sum of two vectors (x,y) (w,z) is igual to the vector (x+w, y+z)
        *  @return Sum Vector
        */
    Vector2D operator+(const Vector2D& right) const;

     /// Method that returns a vector equal to the oposite of another
        /** The oposite of a vector (x,y) is equal to the vector (-x,-y) in this order
        *  @return Oposite Vector 
        */
    // TODO revise
    Vector2D operator-() const;

     /// Method that returns a vector equal to the subtraction of two others
        /** The subtraction of two vectors (x,y) (z,w) is equal to the vector (x-z,y-w) in this order
        *  @return Subtract Vector 
        */
    Vector2D operator-(const Vector2D& right) const;


     /// Method that returns a vector equal to the a vector multiplied by a scalar
        /** A vector (x,y) multiplied by a scalar a is equal to the vector (x * scalar, y*scalar)
        *  @return Scalar Multiplied Vector
        */
    Vector2D operator*(double scalar) const;

     /// Method that returns a vector equal to the a vector multiplied by the inverse of a scalar
        /** A vector (x,y) multiplied by the inverse of a scalar is equal to the vector (x * 1/scalar, y * 1/scalar)
        *  @return Inverse Scalar Multiplied Vector
        */
    Vector2D operator/(double scalar) const;

    /// Method that returns a scalar equal to the inner product of two vectors
        /** By definition the inner product of two vectors (x,y) (w,z) is equal to (x*w) + (y*z)
        *  @return InnerProduct
        */
    double    operator*(const Vector2D &right) const;

    operator std::array<double, 2> () const {
        std::array<double, 2> result {{ x,y }};
        return result;
    }

    operator std::pair<double, double> () const {
        return std::make_pair(x, y);
    }
};

/// Method that returns a vector equal to the a vector multiplied by a scalar
/** A vector (x,y) multiplied by a scalar a is equal to the vector (x * scalar, y*scalar)
  * @return Scalar Multiplied Vector
  */
Vector2D operator*(double scalar, const Vector2D &right);

}  // namespace math
}  // namespace ugdk

#endif  // UGDK_MATH_VECTOR2D_H_
