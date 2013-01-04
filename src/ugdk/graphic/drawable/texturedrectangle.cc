#include <ugdk/config/config.h>
#include "SDL_opengl.h"

#include <ugdk/graphic/drawable/texturedrectangle.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/texture.h>

namespace ugdk {
namespace graphic {

TexturedRectangle::TexturedRectangle(Texture* texture) 
    : size_(static_cast<double>(texture->width()), static_cast<double>(texture->height())), texture_(texture) {}

TexturedRectangle::TexturedRectangle(Texture* texture, const ugdk::math::Vector2D& size) 
    : size_(size), texture_(texture) {}

TexturedRectangle::~TexturedRectangle() {}

void TexturedRectangle::Update(double dt) {}

void TexturedRectangle::Draw(const Geometry& modifier, const VisualEffect& effect) const {
    ugdk::math::Vector2D origin, target(size_);

    origin -= hotspot_;
    target -= hotspot_;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_->gltexture());

    static double TEX_COORD_ONE[]   = { 0.0, 0.0 },
                  TEX_COORD_TWO[]   = { 1.0, 0.0 },
                  TEX_COORD_THREE[] = { 1.0, 1.0 },
                  TEX_COORD_FOUR[]  = { 0.0, 1.0 };
    
    double M[16];
    modifier.AsMatrix4x4(M);
    glPushMatrix();
    glLoadMatrixd(M);

    glColor4dv(effect.color().val);

    glBegin( GL_QUADS ); { //Start quad
        glTexCoord2dv(TEX_COORD_ONE);
        glVertex2dv( origin.val );

        glTexCoord2dv(TEX_COORD_TWO);
        glVertex2d(  target.x, origin.y );

        glTexCoord2dv(TEX_COORD_THREE);
        glVertex2dv( target.val );

        glTexCoord2dv(TEX_COORD_FOUR);
        glVertex2d(  origin.x, target.y );
    } glEnd();

    glPopMatrix();
}

}  // namespace graphic
}  // namespace ugdk
