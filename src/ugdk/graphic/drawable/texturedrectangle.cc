#include <ugdk/config/config.h>
#ifdef ISMAC
    #include "SDL_opengl.h"
#else
    #include <SDL/SDL_opengl.h>
#endif

#include <ugdk/graphic/drawable/texturedrectangle.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/modifier.h>
#include <ugdk/graphic/texture.h>

namespace ugdk {
namespace graphic {

TexturedRectangle::TexturedRectangle(Texture* texture) 
    : size_(static_cast<float>(texture->width()), static_cast<float>(texture->height())), texture_(texture) {}

TexturedRectangle::TexturedRectangle(Texture* texture, const Vector2D& size) 
    : size_(size), texture_(texture) {}

TexturedRectangle::~TexturedRectangle() {}

void TexturedRectangle::Draw(float dt) {
    const Modifier& mod = VIDEO_MANAGER()->CurrentModifier();
    if(!mod.visible()) return;

    Vector2D origin, target(size_);

    if(mod.mirror() & MIRROR_HFLIP) { // Horizontal flip
        origin.x = target.x;
        target.x = 0.0f;
    }
    if(mod.mirror() & MIRROR_VFLIP) { // Vertical flip
        origin.y = target.y;
        target.y = 0.0f;
	}

    origin -= hotspot_;
    target -= hotspot_;

    glColor4fv(mod.color().val);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_->gltexture());

    static float TEX_COORD_ONE[]   = { 0.0f, 0.0f },
                 TEX_COORD_TWO[]   = { 1.0f, 0.0f },
                 TEX_COORD_THREE[] = { 1.0f, 1.0f },
                 TEX_COORD_FOUR[]  = { 0.0f, 1.0f };

	glBegin( GL_QUADS ); { //Start quad
        glTexCoord2fv(TEX_COORD_ONE);
        glVertex2fv( origin.val );

        glTexCoord2fv(TEX_COORD_TWO);
        glVertex2f(  target.x, origin.y );

        glTexCoord2fv(TEX_COORD_THREE);
        glVertex2fv( target.val );

        glTexCoord2fv(TEX_COORD_FOUR);
        glVertex2f(  origin.x, target.y );
    } glEnd();
}

}  // namespace graphic
}  // namespace ugdk
