#include <ugdk/config/config.h>
#include "SDL_opengl.h"

#include <ugdk/graphic/light.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/geometry.h>

namespace ugdk {
namespace graphic {

Light::Light() : color_() {
}

void Light::Draw(const Geometry& modifier) {
	const Texture* light_texture = VIDEO_MANAGER()->light_texture();

    glColor3d(color_.r, color_.g, color_.b);
    
    double M[16];
    modifier.AsMatrix4x4(M);
    glPushMatrix();
    glLoadMatrixd(M);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, light_texture->gltexture());

    const double vertexPositions[] = {
        -dimension_.x, -dimension_.y,
         dimension_.x, -dimension_.y,
         dimension_.x,  dimension_.y,
        -dimension_.x,  dimension_.y,
    };

    static const float texturePositions[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2, GL_DOUBLE, 2*sizeof(*vertexPositions), vertexPositions);
    glTexCoordPointer(2, GL_FLOAT, 2*sizeof(*texturePositions), texturePositions);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}

}  // namespace graphic
}  // namespace ugdk
