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
    
    const float vertexPositions[] = {
        origin.x, origin.y, 0.0f, 1.0f,
        target.x, origin.y, 0.0f, 1.0f,
        target.x, target.y, 0.0f, 1.0f,
        origin.x, target.y, 0.0f, 1.0f,
    };

    double M[16];
    modifier.AsMatrix4x4(M);
    glPushMatrix();
    glLoadMatrixd(M);

    Color color = effect.color() * color_;
    glColor4dv(color.val);
    
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(4, GL_FLOAT, 4*sizeof(float), vertexPositions);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}

}  // namespace graphic
}  // namespace ugdk
