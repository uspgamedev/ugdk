#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>

#include <ugdk/config/config.h>
#ifdef ISMAC
    #include "SDL_image.h"
    #include "SDL_opengl.h"
#else
    #include <SDL/SDL_image.h>
    #include <SDL/SDL_opengl.h>
#endif
#include <ugdk/base/types.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/spritesheet/flexiblespritesheet.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/videomanager.h>

namespace ugdk {
namespace graphic {

FlexibleSpritesheet::FlexibleSpritesheet(Texture* texture) : 
    texture_(texture), frame_size_(1.0f, 1.0f) {}

FlexibleSpritesheet::~FlexibleSpritesheet() {}

const Vector2D FlexibleSpritesheet::render_size() const { 
    return Vector2D((float)texture_->width(), (float)texture_->height());
}

void FlexibleSpritesheet::set_frame_size(const Vector2D& size) {
    frame_size_ = size;
    if(frame_size_.x > 1 || frame_size_.y > 1) {
        // TODO: deprecated conversion!
        frame_size_.x /= texture_->width();
        frame_size_.y /= texture_->height();
    }
    real_frame_size_.x = frame_size_.x * texture_->width();
    real_frame_size_.y = frame_size_.y * texture_->height();
}

void FlexibleSpritesheet::Draw(int frame_number, const Vector2D& hotspot) {
    const Modifier& mod = VIDEO_MANAGER()->CurrentModifier();
    if(!mod.visible()) return;

    /*Frame bounds = VIDEO_MANAGER()->virtual_bounds();
    if (target.x > bounds.right() || target.y > bounds.bottom() ||
        target.x + size.x < bounds.left() || target.y + size.y < bounds.top() ) {
        return false;
    }*/

    Vector2D origin, target = frame_size(frame_number);

    if(mod.mirror() & MIRROR_HFLIP) {
        origin.x = target.x;
        target.x = 0.0f;
    }
    if(mod.mirror() & MIRROR_VFLIP) {
        origin.y = target.y;
        target.y = 0.0f;
    }
    
    origin -= hotspot_ + hotspot;
    target -= hotspot_ + hotspot;

    // Sets the color to tint the image with to the color the image has modified with the color given.
    // Also sets the alpha.
    glColor4fv(mod.color().val);

    int nx = (int)(ceil(1.0f / frame_size_.x));
    float xpos = frame_size_.x * (frame_number % nx);
    float ypos = frame_size_.y * (frame_number / nx);

    float xend = xpos + frame_size_.x;
    float yend = ypos + frame_size_.y;

    GLuint texture = texture_->gltexture();
    if(texture != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
    } else
        glDisable(GL_TEXTURE_2D);

    glBegin( GL_QUADS ); //Start quad
        //Draw square
        glTexCoord2f(xpos, ypos);
        glVertex2f(  origin.x, origin.y );

        glTexCoord2f(xend, ypos);
        glVertex2f(  target.x, origin.y );

        glTexCoord2f(xend, yend);
        glVertex2f(  target.x, target.y );

        glTexCoord2f(xpos, yend);
        glVertex2f(  origin.x, target.y );
    glEnd();
}

// devolve o numero de frames que esta imagem armazena
int FlexibleSpritesheet::frame_count() const {
    return static_cast<int>(std::max(1.0f / (frame_size_.x * frame_size_.y), 1.0f));
}

}  // namespace graphic
}  // namespace ugdk
