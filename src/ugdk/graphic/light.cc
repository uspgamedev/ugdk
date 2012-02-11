#include <ugdk/config/config.h>
#include "SDL_opengl.h"

#include <ugdk/graphic/light.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/texture.h>

namespace ugdk {
namespace graphic {

Light::Light() : color_() {
}

void Light::Draw() {
	const Texture* light_texture = VIDEO_MANAGER()->light_texture();

    glColor3f(color_.r, color_.g, color_.b);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, light_texture->gltexture());

    static double TEX_COORD_ONE[]   = { 0.0, 0.0 },
                 TEX_COORD_TWO[]   = { 1.0, 0.0 },
                 TEX_COORD_THREE[] = { 1.0, 1.0 },
                 TEX_COORD_FOUR[]  = { 0.0, 1.0 };

	glBegin( GL_QUADS ); { //Start quad
        glTexCoord2dv(TEX_COORD_ONE);
        glVertex2d( -dimension_.x, -dimension_.y );

        glTexCoord2dv(TEX_COORD_TWO);
        glVertex2d(  dimension_.x, -dimension_.y );

        glTexCoord2dv(TEX_COORD_THREE);
        glVertex2dv( dimension_.val );

        glTexCoord2dv(TEX_COORD_FOUR);
        glVertex2d( -dimension_.x,  dimension_.y );
    } glEnd();
}

}  // namespace graphic
}  // namespace ugdk
