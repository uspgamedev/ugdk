#include <SDL/SDL_opengl.h>
#include <SDL/SDL_ttf.h>
#include "types.h"
#include "font.h"
#include "image.h"

#define MAX_UNICODE_CODE 18431

namespace ugdk {

Font::Font(Image ** letters, int fontsize, char ident, bool fancy) 
	: size_(fontsize), letters_(letters) {

	id_ = glGenLists(MAX_UNICODE_CODE);
	Vector2D blank;
	for(unsigned int i = 0; i < MAX_UNICODE_CODE; i++) {
		if(letters_[i] == NULL)
			continue;
		glNewList(id_ + i, GL_COMPILE);
			glPushMatrix();
			Vector2D lettersize = letters_[i]->render_size() * (size_ * 0.01f);
			glScalef(lettersize.x, lettersize.y, 1.0f);
			letters_[i]->RawDraw(0);
			glPopMatrix();
			glTranslatef(lettersize.x, 0, 0);
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

Vector2D Font::GetLetterSize(unsigned char letter) {
	if(letters_[letter] == NULL) return Vector2D(0,0);
	return letters_[letter]->render_size() * (size_ * 0.01f);
}

}  // namespace framework
