#include <ugdk/math/frame.h>

namespace ugdk {

// Devolve true se houver colisao entre
// 'frame' e este retangulo
bool Frame::Collides(const Frame& frame) const {
    // se houver colisao no eixo-x...
    if (left() < frame.right() && right() > frame.left()) {
        // e se houver colisao no eixo-y...
        if (top() < frame.bottom() && bottom() > frame.top()) {
            return true;
        }
    }

    return false;
}

}  // namespace framework