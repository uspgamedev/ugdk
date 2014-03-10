#include <ugdk/graphic/textureatlas.h>

#include <ugdk/graphic/texture.h>

namespace ugdk {
namespace graphic {


void TextureAtlas::BoundPiece::ConvertToAtlas(double *u, double *v) const {
    *u = (piece_->position.x + (*u) * piece_->size.x) / atlas_->texture_->width();
    *v = (piece_->position.y + (*v) * piece_->size.y) / atlas_->texture_->height();
}

TextureAtlas::~TextureAtlas() {}

}  // namespace graphic
}  // namespace ugdk
