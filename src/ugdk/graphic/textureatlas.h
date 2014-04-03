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
        const Piece* piece() const { return piece_; }
        void ConvertToAtlas(double *u, double *v) const;
        
      private:
        const TextureAtlas* atlas_;
        const Piece* piece_;
    };
  
    TextureAtlas(std::unique_ptr<Texture>&& texture, std::size_t size)
        :   texture_(std::move(texture))
    {
        pieces_.reserve(size);
    }
    ~TextureAtlas();
    
    std::size_t AddPiece(math::Vector2D& pos, math::Vector2D& size) {
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
