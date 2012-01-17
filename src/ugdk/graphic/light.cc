#include <SDL/SDL_opengl.h>

#include <ugdk/graphic/light.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/texture.h>

namespace ugdk {

Light::Light() : color_() {
}

void Light::Draw() {
	const Texture* light_texture = VIDEO_MANAGER()->light_texture();

    glColor3f(color_.r, color_.g, color_.b);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, light_texture->gltexture());

    static float TEX_COORD_ONE[]   = { 0.0f, 0.0f },
                 TEX_COORD_TWO[]   = { 1.0f, 0.0f },
                 TEX_COORD_THREE[] = { 1.0f, 1.0f },
                 TEX_COORD_FOUR[]  = { 0.0f, 1.0f };

	glBegin( GL_QUADS ); { //Start quad
        glTexCoord2fv(TEX_COORD_ONE);
        glVertex2f( -dimension_.x, -dimension_.y );

        glTexCoord2fv(TEX_COORD_TWO);
        glVertex2f(  dimension_.x, -dimension_.y );

        glTexCoord2fv(TEX_COORD_THREE);
        glVertex2fv( dimension_.val );

        glTexCoord2fv(TEX_COORD_FOUR);
        glVertex2f( -dimension_.x,  dimension_.y );
    } glEnd();
}

}  // namespace framework
