#include <ugdk/config/config.h>
#include "SDL_opengl.h"

#include <ugdk/graphic/drawable/texturedrectangle.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/modifier.h>
#include <ugdk/graphic/texture.h>

namespace ugdk {
namespace graphic {

TexturedRectangle::TexturedRectangle(Texture* texture) 
    : size_(static_cast<double>(texture->width()), static_cast<double>(texture->height())), texture_(texture) {}

TexturedRectangle::TexturedRectangle(Texture* texture, const Vector2D& size) 
    : size_(size), texture_(texture) {}

TexturedRectangle::~TexturedRectangle() {}

void TexturedRectangle::Draw(double dt) {
    const Modifier& mod = VIDEO_MANAGER()->CurrentModifier();
    if(!mod.visible()) return;

    Vector2D origin, target(size_);

    if(mod.mirror() & MIRROR_HFLIP) { // Horizontal flip
        origin.x = target.x;
        target.x = 0.0;
    }
    if(mod.mirror() & MIRROR_VFLIP) { // Vertical flip
        origin.y = target.y;
        target.y = 0.0;
	}

    origin -= hotspot_;
    target -= hotspot_;

    glColor4dv(mod.color().val);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_->gltexture());

    static double TEX_COORD_ONE[]   = { 0.0, 0.0 },
                 TEX_COORD_TWO[]   = { 1.0, 0.0 },
                 TEX_COORD_THREE[] = { 1.0, 1.0 },
                 TEX_COORD_FOUR[]  = { 0.0, 1.0 };

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
}

}  // namespace graphic
}  // namespace ugdk
