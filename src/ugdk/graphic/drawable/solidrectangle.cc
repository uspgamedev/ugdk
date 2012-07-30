#include <ugdk/config/config.h>
#include "SDL_opengl.h"

#include <ugdk/graphic/drawable/solidrectangle.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/modifier.h>

namespace ugdk {
namespace graphic {

SolidRectangle::SolidRectangle(const Vector2D& size) : size_(size) {}

SolidRectangle::~SolidRectangle() {}
    
void SolidRectangle::Update(double dt) {}

void SolidRectangle::Draw() const {
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

    Color color(mod.color().r * color_.r, mod.color().g * color_.g, mod.color().b * color_.b, mod.color().a * color_.a);

    glColor4dv(color.val);

    glDisable(GL_TEXTURE_2D);
    glBegin( GL_QUADS ); { //Start quad
        glVertex2dv( origin.val );
        glVertex2d(  target.x, origin.y );
        glVertex2dv( target.val );
        glVertex2d(  origin.x, target.y );
    } glEnd();
}

}  // namespace graphic
}  // namespace ugdk
