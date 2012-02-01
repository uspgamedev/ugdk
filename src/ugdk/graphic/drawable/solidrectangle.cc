#ifdef ISMAC
    #include "SDL_opengl.h"
#else
    #include <SDL/SDL_opengl.h>
#endif

#include <ugdk/graphic/drawable/solidrectangle.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/modifier.h>

namespace ugdk {

SolidRectangle::SolidRectangle(const Vector2D& size) : size_(size) {}

SolidRectangle::~SolidRectangle() {}

void SolidRectangle::Draw(float dt) {
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

    Color color(mod.color().r * color_.r, mod.color().g * color_.g, mod.color().b * color_.b, mod.color().a * color_.a);

    glColor4fv(color.val);

    glDisable(GL_TEXTURE_2D);
	glBegin( GL_QUADS ); { //Start quad
        glVertex2fv( origin.val );
        glVertex2f(  target.x, origin.y );
        glVertex2fv( target.val );
        glVertex2f(  origin.x, target.y );
    } glEnd();
}

}  // namespace ugdk
