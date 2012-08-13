#include <ugdk/config/config.h>
#include "SDL_opengl.h"

#include <ugdk/graphic/drawable/solidrectangle.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/modifier.h>

namespace ugdk {
namespace graphic {

SolidRectangle::SolidRectangle(const ugdk::math::Vector2D& size) : size_(size) {}

SolidRectangle::~SolidRectangle() {}
    
void SolidRectangle::Update(double dt) {}

void SolidRectangle::Draw() const {
    ugdk::math::Vector2D origin, target(size_);
    origin -= hotspot_;
    target -= hotspot_;

    glColor4dv(color_.val);

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
