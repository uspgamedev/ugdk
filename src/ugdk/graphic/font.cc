#include <ugdk/config/config.h>
#ifdef ISMAC
    #include "SDL_opengl.h"
    #include "SDL_ttf.h"
#else
    #include <SDL/SDL_opengl.h>
    #include <SDL/SDL_ttf.h>
#endif
#include <ugdk/base/types.h>
#include <ugdk/graphic/font.h>
#include <ugdk/graphic/texture.h>

#define MAX_UNICODE_CODE 18431

namespace ugdk {

Font::Font(Texture** letters, int fontsize, char ident, bool fancy) 
	: size_(fontsize), letters_(letters) {

    static float TEX_COORD_ONE[]   = { 0.0f, 0.0f },
                 TEX_COORD_TWO[]   = { 1.0f, 0.0f },
                 TEX_COORD_THREE[] = { 1.0f, 1.0f },
                 TEX_COORD_FOUR[]  = { 0.0f, 1.0f };

	id_ = glGenLists(MAX_UNICODE_CODE);
	Vector2D blank;
	for(unsigned int i = 0; i < MAX_UNICODE_CODE; i++) {
		if(letters_[i] == NULL)
			continue;
		glNewList(id_ + i, GL_COMPILE);
            float targetx = letters_[i]->width()  * size_ * 0.01f;
            float targety = letters_[i]->height() * size_ * 0.01f;
            glBindTexture(GL_TEXTURE_2D, letters_[i]->gltexture());
			glBegin( GL_QUADS ); { //Start quad
                glTexCoord2fv(TEX_COORD_ONE);
                glVertex2f(    0.0f,    0.0f );

                glTexCoord2fv(TEX_COORD_TWO);
                glVertex2f( targetx,    0.0f );

                glTexCoord2fv(TEX_COORD_THREE);
                glVertex2f( targetx, targety );

                glTexCoord2fv(TEX_COORD_FOUR);
                glVertex2f(    0.0f, targety );
            } glEnd();
			glTranslatef(targetx, 0.0f, 0.0f);
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
Vector2D Font::GetLetterSize(unsigned char letter) {
	if(letters_[letter] == NULL) return Vector2D(0,0);
	return letters_[letter]->render_size() * (size_ * 0.01f);
}*/

Vector2D Font::GetLetterSize(wchar_t letter) {
	if(letters_[letter] == NULL) return Vector2D(0,0);
    return Vector2D(letters_[letter]->width() * (size_ * 0.01f), letters_[letter]->height() * (size_ * 0.01f));
}

}  // namespace framework
