#include <ugdk/config/config.h>
#include "SDL_opengl.h"
#include "SDL_ttf.h"
#include <ugdk/base/types.h>
#include <ugdk/graphic/font.h>
#include <ugdk/graphic/texture.h>

#define MAX_UNICODE_CODE 18431

namespace ugdk {
namespace graphic {

Font::Font(Texture** letters, int fontsize, char ident, bool fancy) 
	: size_(fontsize), letters_(letters) {

    static GLdouble TEX_COORD_ONE[]   = { 0.0, 0.0 },
                 TEX_COORD_TWO[]   = { 1.0, 0.0 },
                 TEX_COORD_THREE[] = { 1.0, 1.0 },
                 TEX_COORD_FOUR[]  = { 0.0, 1.0 };

	id_ = glGenLists(MAX_UNICODE_CODE);
	ugdk::math::Vector2D blank;
	for(unsigned int i = 0; i < MAX_UNICODE_CODE; i++) {
		if(letters_[i] == nullptr)
			continue;
		glNewList(id_ + i, GL_COMPILE);
            double targetx = letters_[i]->width()  * size_ * 0.01;
            double targety = letters_[i]->height() * size_ * 0.01;
            glBindTexture(GL_TEXTURE_2D, letters_[i]->gltexture());
			glBegin( GL_QUADS ); { //Start quad
                glTexCoord2dv(TEX_COORD_ONE);
                glVertex2d(    0.0,    0.0 );

                glTexCoord2dv(TEX_COORD_TWO);
                glVertex2d( targetx,    0.0 );

                glTexCoord2dv(TEX_COORD_THREE);
                glVertex2d( targetx, targety );

                glTexCoord2dv(TEX_COORD_FOUR);
                glVertex2d(    0.0, targety );
            } glEnd();
			glTranslated(targetx, 0.0, 0.0);
		glEndList();
	}
	switch(ident) {
		case 'l':
			ident_ = LEFT;
			break;
		case 'r':
			ident_ = RIGHT;
			break;
		case 'c':
		default:
			ident_ = CENTER;
			break;
	}
	fancy_ = fancy;
}

Font::~Font() {
	glDeleteLists(id_, MAX_UNICODE_CODE);
}

/*FUCKYOU ANSI
ugdk::math::Vector2D Font::GetLetterSize(unsigned char letter) {
	if(letters_[letter] == nullptr) return ugdk::math::Vector2D(0,0);
	return letters_[letter]->render_size() * (size_ * 0.01);
}*/

ugdk::math::Vector2D Font::GetLetterSize(wchar_t letter) {
	if(letters_[letter] == nullptr) return ugdk::math::Vector2D(0,0);
    return ugdk::math::Vector2D(letters_[letter]->width() * (size_ * 0.01), letters_[letter]->height() * (size_ * 0.01));
}

}  // namespace graphic
}  // namespace ugdk
