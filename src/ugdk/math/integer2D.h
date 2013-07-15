#ifndef UGDK_MATH_INTEGER2D_H_
#define UGDK_MATH_INTEGER2D_H_

#include <ugdk/common/types.h>
#include <ugdk/math.h>

#ifdef SWIG
#pragma SWIG nowarn=312
#endif

namespace ugdk {

namespace math {

enum RotDeg {
  NINETY, TWO_NINETY, THREE_NINETY
};

// 2 dimension vectors, using doubles.
class Integer2D {
  public:
    /// Initializes both fields with 0
    /** Create the integer (x,y) with x = 0 and y =0
    */
    Integer2D() : x(0), y(0) {}

    /// Initializes both fields with val.
    /** Create the integer (x,y) with x = val and y = val being val a constant
    *  @param val is a constant
    */
    explicit Integer2D(int val) : x(val), y(val) {}

    /// Initializes both fields with given values.
    /** Create the integer (x,y) with x = x-value and y = y-value
    *  @param x is the x-value argument
    *  @param y is the y-value argument
    */
    Integer2D(int _x, int _y) : x(_x), y(_y) {}

    /// Copy constructor from ugdk::math::Vector2D.
    Integer2D(const ugdk::math::Vector2D& vec2d);

    ~Integer2D() { }

    // Fields are accessible as x/y or val[0] and val[1].
    union {
        struct { int x, y;   };
        struct { int val[2]; };
    };

    int get_x() const { return x; }
    int get_y() const { return y; }
    void set_x(int x_) { x = x_; }
    void set_y(int y_) { y = y_; }


    /// Returns the norm-1 of this integer.
    /** The norm-1 of a integer (x,y) is |x| + |y|.
    *  @return Norm-1
    *  @see Length()
    */
    int NormOne() const;

    /// Returns the norm-2 of this integer
    /** The norm-2 of a integer (x,y) is sqrt( x^2 + y^2 )
    *  @return Norm-2
    *  @see NormOne()
    *  @see LenghtSquared()
    */
    double Length() const;
    
    /// Returns the norm-2 squared
    /** The norm-2 squared of a integer (x,y) is (x^2 + y^2)
    *  @return Norm-2 Squared
    *  @see NormOne()
    *  @see Lenght()
    */
    double LengthSquared() const { return (x*x) + (y*y); }

    /// Returns the angle (in radians) of this integer
    /** Return the angle (in radians) associated to this integer
    *  @return Angle
    */
    double Angle() const;

    /// Returns a new integer equal to this integer rotated by "angle" (in radians) counter-clockwise.
    /** The integer rotated of a integer (x,y) is (x * cos(angle) - y * sin(angle) , x * sin(angle) + y * cos(angle));  angle in radians
    *  @return Rotated Integer
    *  @see Angle
    */
    void Rotate(RotDeg rotdeg);
    
    /// Returns a new integer equal to this integer rotated by "angle" (in radians) counter-clockwise.
    /** The integer rotated of a integer (x,y) is (x * cos(angle) - y * sin(angle) , x * sin(angle) + y * cos(angle));  angle in radians
    *  @return Rotated Integer
    *  @see Angle
    */
    void Rotate(RotDeg rotdeg, const Integer2D& center) {
        *this -= center;
        Rotate(rotdeg);
        *this += center;
    }

    /// Returns a new integer equal to this integer rotated by "angle" (in radians) counter-clockwise.
    /** The integer rotated of a integer (x,y) is (x * cos(angle) - y * sin(angle) , x * sin(angle) + y * cos(angle));  angle in radians
    *  @return Rotated Integer
    *  @see Angle
    */
    Integer2D Rotated(RotDeg rotdeg) const;

    /// Returns a new integer equal to this integer rotated by "angle" (in radians) counter-clockwise.
    /** The integer rotated of a integer (x,y) is (x * cos(angle) - y * sin(angle) , x * sin(angle) + y * cos(angle));  angle in radians
    *  @return Rotated Integer
    *  @see Angle
    */
    Integer2D Rotated(RotDeg rotdeg, const Integer2D& center) const {
        return center + (*this - center).Rotated(rotdeg);
    }
    
    /// Mirrors this Integer2D (in-place) by the "axis" axis. Include types.h and use "using namespace ugdk::enums".
    /** @param axis Either mirroraxis::HORZ, mirroraxis::VERT, mirroraxis::DIAG_UP, or mirroraxis::DIAG_DOWN.
     *  @see Mirrored
     */
    void Mirror(const ugdk::enums::mirroraxis::MirrorAxis mirror);
    
    /// Returns a new Integer2D, mirrored by the "axis" axis. Include types.h and use "using namespace ugdk::enums".
    /** @param axis Either mirroraxis::HORZ, mirroraxis::VERT, mirroraxis::DIAG_UP, or mirroraxis::DIAG_DOWN.
     *  @see Mirror
     */
    Integer2D Mirrored(const ugdk::enums::mirroraxis::MirrorAxis mirror) const;

    /// In-place integer multiplication, coordinate by coordinate.
    /** @see Multiplied
     *  @see Scale
     */
    void Multiply(const Integer2D& multiplier);
    
    /// Returns a new Integer2D equal to this multiplied coordinate by coordinate by "multiplier".
    /** @see Multiply
     *  @see Scaled
     */
    Integer2D Multiplied(const Integer2D& multiplier) const;
    
    /// In-place integer division, coordinate by coordinate.
    /** @see Divided
     *  @see Scale
     */
    void Divide(const Integer2D& divider);
    
    /// Returns a new Integer2D equal to this divided coordinate by coordinate by "multiplier".
    /** @see Divide
     *  @see Scale
     */
    Integer2D Divided(const Integer2D& divider) const;
    
    /// Multiplies in-place by "multiplier", and then divides by "divisor".
    /** @see Scaled
     *  @see Multiply
     *  @see Divide
     */
    void Scale(const Integer2D& multiplier, const Integer2D& divisor);
    
    /// Returns a new Integer2D, multiplied by "multiplier", and divided by "divisor".
    /** @see Scale
     *  @see Multiply
     *  @see Divide
     */
    Integer2D Scaled(const Integer2D& multiplier, const Integer2D& divisor) const;

    /// In-place remainder (coordinate by coordinate) of integer division by "divisor".
    /** @see Remainder
     */
    void Mod(const Integer2D& divisor);
    
    /// Returns a new Integer2D that is equal to the remainder of the integer division by "divisor".
    /** @see Mod
     */
    Integer2D Remainder(const Integer2D& divisor) const;


    // Operators methods

    // TODO document
    Integer2D& operator+=(const Integer2D &other);
    
    // TODO document
    Integer2D& operator-=(const Integer2D &other);
    
    // TODO document
    Integer2D& operator*=(int scalar);
    
    // TODO document
    Integer2D& operator/=(int scalar);

    // TODO document
    Integer2D& operator%=(int scalar);

    /// Method that returns a integer equal to the sum of two others
    /** The sum of two vectors (x,y) (w,z) is igual to the integer (x+w, y+z)
      *  @return Sum Integer
      */
    Integer2D operator+(const Integer2D &right) const;

    /// Method that returns a integer equal to the oposite of another
    /** The oposite of a integer (x,y) is equal to the integer (-x,-y) in this order
      * @return Oposite Integer 
      */
    // TODO revise
    Integer2D operator-() const;

    /// Method that returns a integer equal to the subtraction of two others
    /** The subtraction of two vectors (x,y) (z,w) is equal to the integer (x-z,y-w) in this order
      * @return Subtract Integer 
      */
    Integer2D operator-(const Integer2D &right) const;


    /// Method that returns a integer equal to the a integer multiplied by a int
    /** A integer (x,y) multiplied by a scalar a is equal to the integer (x * scalar, y*scalar)
      * @return Scalar Multiplied Integer
      */
    Integer2D operator*(int scalar) const;
     
    /// Method that returns a vector equal to the a integer multiplied by a double
    /** A Vector (x,y) multiplied by a scalar a is equal to the vector (x * scalar, y * scalar)
      * @return Scalar Multiplied Vector
      */
    Vector2D operator*(double scalar) const;

     /// Method that returns a integer equal to the a integer multiplied by the inverse of a scalar
        /** A integer (x,y) multiplied by the inverse of a scalar is equal to the integer (x * 1/scalar, y * 1/scalar)
        *  @return Inverse Scalar Multiplied Integer
        */
    Integer2D operator/(int scalar) const;

     /// Remainder of integer division by "scalar" in each coordinate.
    Integer2D operator%(int scalar) const;

    /// Method that returns a scalar equal to the inner product of two vectors
        /** By definition the inner product of two vectors (x,y) (w,z) is equal to (x*w) + (y*z)
        *  @return InnerProduct
        */
    int    operator*(const Integer2D &right) const;
    
    /// Remainder of integer division by "scalar" coordinate by coordinate.
    Integer2D operator%(const Integer2D& right) const;

    /// Checks if their coordinates are equal, coordinate by coordinate.
    bool operator==(const Integer2D& right) const { 
        return this->x == right.x && this->y == right.y;
    }

    /// True when operator== is false.
    bool operator!=(const Integer2D& right) const { return !(right == *this); }
};

/// Method that returns a integer equal to the a integer multiplied by a scalar
/** A integer (x,y) multiplied by a scalar a is equal to the integer (x * scalar, y*scalar)
  * @return Scalar Multiplied Integer
  */
Integer2D operator*(int scalar, const Integer2D &right);

/// Method that returns a vector equal to the a integer multiplied by a double
/** A Vector (x,y) multiplied by a scalar a is equal to the vector (x * scalar, y * scalar)
  * @return Scalar Multiplied Vector
  */
Vector2D operator*(double scalar, const Integer2D &right);

}  // namespace math
}  // namespace ugdk

#endif  // UGDK_MATH_INTEGER2D_H_
