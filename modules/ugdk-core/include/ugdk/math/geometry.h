#ifndef UGDK_MATH_GEOMETRY_H_
#define UGDK_MATH_GEOMETRY_H_

#include <ugdk/math/vector2D.h>

#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/vec2.hpp>


namespace ugdk {
namespace math {

class Geometry {
  public:
    /// Creates an identity Geometry;
    Geometry() : rotation_(0.0), matrix_(1.0f) {}

    /// Creates a new Geometry object with the specified values. 
    /**
     ** @param offset The offset of the image.
     ** @param scale The size modifiers for the image. X and Y values can be set
     ** independently.
     ** @param rot Rotation angle in radians. 0 points to the right, increases in
     ** counterclockwise fashion.
     */
    Geometry(const glm::vec2& offset, const glm::vec2& scale = {1.0f, 1.0f}, double rot = 0.0)
    : rotation_(-rot),
    matrix_(float(scale.x*cos(-rot)), -float(scale.x*sin(-rot)), 0.0f, 0.0f, // First column
    float(scale.y*sin(-rot)),  float(scale.y*cos(-rot)), 0.0f, 0.0f,
    0.0f,                      0.0f, 1.0f, 0.0f,
    float(offset.x),           float(offset.y), 0.0f, 1.0f) {}
    
    Geometry(const math::Vector2D& offset, const math::Vector2D scale = math::Vector2D(1.0, 1.0), double rot = 0.0)
    : Geometry(glm::vec2(offset.x, offset.y), glm::vec2(scale.x, scale.y), rot) {}
    
    // Destructor
    ~Geometry() {}

    double rotation() const { return rotation_; }

    /// Calculates by how much this Geometry translates the rendering.
    ugdk::math::Vector2D offset() const { 
        return math::Vector2D(matrix_[3].x, matrix_[3].y);
    }

    void set_offset(const ugdk::math::Vector2D& _offset) {
        ChangeOffset(_offset);
    }

    void ChangeOffset(const math::Vector2D& _offset) {
        matrix_[3] = glm::vec4(_offset.x, _offset.y, 0.0, 1.0);
    }

    void Compose(const Geometry& other) {
        rotation_ += other.rotation_;
        matrix_ *= other.matrix_;
    }

    Geometry& operator *= (const Geometry& rhs) {
        Compose(rhs);
        return *this;
    }
    
    Geometry operator * (const Geometry& rhs) const {
        Geometry result(*this);
        result.Compose(rhs);
        return result;
    }

    const glm::mat4& AsMat4() const {
        return matrix_;
    }

    math::Vector2D CalculateScale() const {
        double c = cos(rotation_);
        if(c > 0.5)
            return math::Vector2D( matrix_[0].x / cos(rotation_), matrix_[1].y / cos(rotation_));
        else
            return math::Vector2D(-matrix_[1].x / sin(rotation_), matrix_[0].y / sin(rotation_));
    }

  private:
    double rotation_;
    glm::mat4 matrix_;
};

}  // namespace math
}  // namespace ugdk

#endif /* UGDK_MATH_MODIFIER_H_ */
