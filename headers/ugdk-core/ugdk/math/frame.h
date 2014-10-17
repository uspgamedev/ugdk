#ifndef UGDK_MATH_FRAME_H_
#define UGDK_MATH_FRAME_H_

namespace ugdk {
namespace math {

// Representa um quadro (retangulo) na tela e/ou numa imagem.
// Util para trabalhar com animacoes, detectar colisoes
// (bounding box), etc.
//
// left, top, right e bottom sao usados para determinar as
// coordenadas do retangulo.
//
//   (left,top)
//        +------------------+
//        |                  |
//        |                  |
//        +------------------+
//                        (right,bottom)

/** @brief Represents a frame on the screen or an image.

    Represents a rectangular frame on the screen or an image.
    Useful for working with animations, collision detection, etc.
    left and top represent the frame's upper-left corner, while
    right and bottom represent the lower-right coordinates.
*/
class Frame {
  public:
    // Construtores e destrutores
    /// Creates an empty frame at (0, 0).
    Frame() : left_(0), top_(0), right_(0), bottom_(0) { }
    /// Creates a frame with the given coordinates.
  /**
     @param left The left margin
     @param top The upper margin
     @param right The right margin
     @param bottom The lower margin
  */
    Frame(double _left, double _top, double _right, double _bottom) :
        left_(_left), top_(_top), right_(_right), bottom_(_bottom) { }
    ~Frame() { }

    // Utilidades inline
    /// Returns the width of the frame.
    double width() const { return right_ - left_; }
    ///Returns the height of the frame.
    double height() const { return bottom_ - top_; }

    // Accessors e mutators
    /**@name Accessors and Mutators
    */
    /**@{
     */
    void set_left(double _left) { left_ = _left; }
    void set_top(double _top) { top_ = _top; }
    void set_right(double _right) { right_ = _right; }
    void set_bottom(double _bottom) { bottom_ = _bottom; }

    double left() const { return left_; }
    double top() const { return top_; }
    double right() const { return right_; }
    double bottom() const { return bottom_; }
    /** @}
     */
    // Devolve true se houver colisao entre
    // 'frame' e este retangulo.
    /// Returns whether this frame and the other frame are colliding.
    bool Collides(const math::Frame& frame) const;

  private:
    double left_, top_, right_, bottom_;
};

} // namespace math
} // namespace ugdk

#endif  // UGDK_MATH_FRAME_H_
