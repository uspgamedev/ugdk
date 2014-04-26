#include <ugdk/graphic/textureatlas.h>

#include <ugdk/graphic/texture.h>

namespace ugdk {
namespace graphic {


void TextureAtlas::BoundPiece::ConvertToAtlas(float *u, float *v) const {
    ConvertToAtlas(*u, *v, u, v);
}
        
void TextureAtlas::BoundPiece::ConvertToAtlas(float in_u, float in_v, float *out_u, float *out_v) const {
    *out_u = (piece_->position.x + (in_u) * piece_->size.x) / atlas_->texture_->width();
    *out_v = (piece_->position.y + (in_v) * piece_->size.y) / atlas_->texture_->height();
}

TextureAtlas::TextureAtlas(std::unique_ptr<Texture>&& texture, std::size_t size)
:   texture_(std::move(texture))
{
    pieces_.reserve(size);
}

TextureAtlas::~TextureAtlas() {}

}  // namespace graphic
}  // namespace ugdk
