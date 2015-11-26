#ifndef UGDK_GRAPHIC_TEXTUREALTAS_H_
#define UGDK_GRAPHIC_TEXTUREALTAS_H_

#include <ugdk/graphic.h>
#include <ugdk/math/integer2D.h>
#include <ugdk/system/exceptions.h>

#include <memory>
#include <vector>
#include <unordered_map>

#ifdef SWIG
// Nested class not supported
#pragma SWIG nowarn=325
#endif

namespace ugdk {
namespace graphic {

class TextureAtlas {
  struct Piece {
    Piece(const math::Integer2D& pos, const math::Integer2D& s)
        : position(pos), size(s), trimmed_size(s)
        , horizontal_flip(false), vertical_flip(false), rotated_90_clockwise(false) {}
    math::Integer2D position, size, trimmed_size, offset;
    bool horizontal_flip;
    bool vertical_flip;
    bool rotated_90_clockwise;
  };
  public:
    class BoundPiece {
      public:
        BoundPiece(const TextureAtlas* atlas, const Piece* piece) : atlas_(atlas), piece_(piece) {}
        ~BoundPiece() {}
        
        const TextureAtlas* atlas() const { return atlas_; }
        const math::Integer2D& position() const { return piece_->position; }
        const math::Integer2D& size() const { return piece_->size; }
        const math::Integer2D& trimmed_size() const { return piece_->trimmed_size; }
        const math::Integer2D& offset() const { return piece_->offset; }

        void ConvertToAtlas(float *u, float *v) const;
        void ConvertToAtlas(float in_u, float in_v, float *out_u, float *out_v) const;
        
      private:
        const TextureAtlas* atlas_;
        const Piece* piece_;
    };
  
    TextureAtlas(const graphic::GLTexture* texture, std::size_t size);
    ~TextureAtlas();

    static TextureAtlas* LoadFromFile(const std::string& filepath);
    
    std::size_t AddPiece(const std::string& name, const math::Integer2D& pos, const math::Integer2D& size) {
        if (names_.find(name) != names_.end())
            throw system::BaseException("Piece already declared: %s", name.c_str());

        pieces_.emplace_back(pos, size);
        std::size_t frame_number = pieces_.size() - 1;
        names_[name] = frame_number;
        return frame_number;
    }
    
    math::Integer2D size() const;
    size_t piece_num() const {
        return pieces_.size();
    }
    const graphic::GLTexture* texture() const {
        return texture_;
    }

    std::size_t NameToId(const std::string& name) const {
        return names_.at(name);
    }

#ifndef SWIG
    BoundPiece PieceAt(size_t i) const {
        return BoundPiece(this, &pieces_[i]);
    }

    BoundPiece PieceAt(const std::string& name) const { 
        return PieceAt(NameToId(name));
    }
#endif

  private:
    const graphic::GLTexture* texture_;
    std::vector<Piece> pieces_;
    std::unordered_map<std::string, std::size_t> names_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_TEXTUREALTAS_H_
