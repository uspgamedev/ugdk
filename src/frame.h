#ifndef HORUSEYE_FRAMEWORK_FRAME_H_
#define HORUSEYE_FRAMEWORK_FRAME_H_

namespace ugdk {

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
class Frame {
  public:
    // Construtores e destrutores
    Frame() : left_(0), top_(0), right_(0), bottom_(0) { }
    Frame(float left, float top, float right, float bottom) :
        left_(left), top_(top), right_(right), bottom_(bottom) { }
    ~Frame() { }

    // Utilidades inline
    float width() const { return right_ - left_; }
    float height() const { return bottom_ - top_; }

    // Accessors e mutators
    float left() const { return left_; }
    void set_left(float left) { left_ = left; }
    float top() const { return top_; }
    void set_top(float top) { top_ = top; }
    float right() const { return right_; }
    void set_right(float right) { right_ = right; }
    float bottom() const { return bottom_; }
    void set_bottom(float bottom) {bottom_ = bottom; }

    // Devolve true se houver colisao entre
    // 'frame' e este retangulo.
    bool Collides(const Frame& frame) const;

  private:
    float left_, top_, right_, bottom_;
};

}  // namespace framework

#endif  // HORUSEYE_FRAMEWORK_FRAME_H_
