#ifndef UGDK_GRAPHIC_TEXTUREALTAS_H_
#define UGDK_GRAPHIC_TEXTUREALTAS_H_

#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>

#include <memory>
#include <vector>

namespace ugdk {
namespace graphic {

class TextureAtlas {
  struct Piece {
    Piece(const math::Vector2D& pos, const math::Vector2D& s) : position(pos), size(s) {}
    math::Vector2D position, size;
  };
  public:
    class BoundPiece {
      public:
        BoundPiece(const TextureAtlas* atlas, const Piece* piece) : atlas_(atlas), piece_(piece) {}
        ~BoundPiece() {}
        
        const TextureAtlas* atlas() const { return atlas_; }
        const math::Vector2D& position() const { return piece_->position; }
        const math::Vector2D& size() const { return piece_->size; }

        void ConvertToAtlas(float *u, float *v) const;
        void ConvertToAtlas(float in_u, float in_v, float *out_u, float *out_v) const;
        
      private:
        const TextureAtlas* atlas_;
        const Piece* piece_;
    };
  
    TextureAtlas(std::unique_ptr<Texture>&& texture, std::size_t size);
    ~TextureAtlas();
    
    std::size_t AddPiece(const math::Vector2D& pos, const math::Vector2D& size) {
        pieces_.emplace_back(pos, size);
        return pieces_.size() - 1;
    }
    
    const Texture* texture() const { return texture_.get(); }
    BoundPiece PieceAt(size_t i) const { return BoundPiece(this, &pieces_[i]); }

  private:
    std::unique_ptr<Texture> texture_;
    std::vector<Piece> pieces_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_TEXTUREALTAS_H_
