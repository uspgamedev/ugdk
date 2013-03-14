#include <ugdk/config/config.h>
#include "SDL_opengl.h"

#include <ugdk/graphic/drawable/solidrectangle.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/visualeffect.h>

namespace ugdk {
namespace graphic {

SolidRectangle::SolidRectangle(const ugdk::math::Vector2D& size) : size_(size) {}

SolidRectangle::~SolidRectangle() {}
    
void SolidRectangle::Update(double dt) {}

void SolidRectangle::Draw(const Geometry& modifier, const VisualEffect& effect) const {
    ugdk::math::Vector2D origin, target(size_);
    
    origin -= hotspot_;
    target -= hotspot_;
    
    const double vertexPositions[] = {
        origin.x, origin.y,
        target.x, origin.y,
        target.x, target.y,
        origin.x, target.y,
    };

    float M[16];
    modifier.AsMatrix4x4(M);
    glPushMatrix();
    glLoadMatrixf(M);

    Color color = effect.color() * color_;
    glColor4dv(color.val);
    
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(2, GL_DOUBLE, 2*sizeof(*vertexPositions), vertexPositions);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}

}  // namespace graphic
}  // namespace ugdk
