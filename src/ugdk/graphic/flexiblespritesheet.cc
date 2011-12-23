#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include <ugdk/math/frame.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/base/engine.h>
#include <ugdk/base/types.h>
#include <ugdk/graphic/flexiblespritesheet.h>

namespace ugdk {

FlexibleSpritesheet::FlexibleSpritesheet() : 
    texture_(0), texture_width_(10), texture_height_(10), frame_size_(1.0f, 1.0f) {}

FlexibleSpritesheet::~FlexibleSpritesheet() {
	if(texture_ != 0) {
        glDeleteTextures(1, &texture_);
		texture_ = 0;
	}
}

void FlexibleSpritesheet::Draw(int frame_number) {
    const Modifier& mod = VIDEO_MANAGER()->CurrentModifier();

    /*Frame bounds = VIDEO_MANAGER()->virtual_bounds();
    if (target.x > bounds.right() || target.y > bounds.bottom() ||
        target.x + size.x < bounds.left() || target.y + size.y < bounds.top() ) {
        return false;
    }*/

    Vector2D origin, target(texture_width_ * frame_size_.x, texture_height_ * frame_size_.y);

    if(mod.mirror() & MIRROR_HFLIP) {
		// Flip the image horizontally and move it to the right, so it renders at the same place.
        origin.x = target.x;
        target.x = 0.0f;
    }
    if(mod.mirror() & MIRROR_VFLIP) {
		// Flip the image vertically and move it down, so it renders at the same place.
        origin.y = target.y;
        target.y = 0.0f;
	}
    
    origin -= hotspot_;
    target -= hotspot_;

	// Sets the color to tint the image with to the color the image has modified with the color given.
	// Also sets the alpha.
    glColor4f(mod.color().r, mod.color().g, mod.color().b, mod.alpha());

    int nx = (int)(ceil(1.0f / frame_size_.x));
    float xpos = frame_size_.x * (frame_number % nx);
    float ypos = frame_size_.y * (frame_number / nx);

    float xend = xpos + frame_size_.x;
    float yend = ypos + frame_size_.y;

    GLuint texture = static_cast<GLuint>(texture_);
    if(texture != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
    } else
        glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
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
	glDisable(GL_BLEND);
}

// devolve o numero de frames que esta imagem armazena
int FlexibleSpritesheet::FrameCount() const {
    Vector2D size = frame_size();
    return static_cast<int>(std::max(1.0f / (size.x * size.y), 1.0f));
}

// Loads the texture from the given surface. Surface is used only as a
// reference and necessary data is copied.
// Returns true on success, false otherwise.
static bool ConvertSurfaceToTexture(SDL_Surface* data, uint32* texture_, int* texture_width_, int* texture_height_) {
    if(data == NULL) {
        fprintf(stderr, "Error: LoadSurface - No Data\n");
        return false;
    }
    if(*texture_ != 0) {
        glDeleteTextures(1, texture_);
    }

    Uint8 *raw = static_cast<Uint8*>( malloc( data->w * data->h * 4 ) );
    Uint8 *dstPixel = raw;

    SDL_LockSurface( data );
    int bpp = data->format->BytesPerPixel;
    Uint8 *srcPixel;
    Uint32 truePixel;

    for ( int i = 0; i < data->h ; i++ ) {
        for ( int j = 0 ; j < data->w ; j++ ) {
            srcPixel = (Uint8 *)data->pixels + i * data->pitch + j * bpp;
            switch (bpp) {
            case 1:
                truePixel = *srcPixel;
                break;

            case 2:
                truePixel = *srcPixel;
                break;

            case 3:
                if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                    truePixel = srcPixel[0] << 16 | srcPixel[1] << 8 | srcPixel[2];
                } else {
                    truePixel = srcPixel[0] | srcPixel[1] << 8 | srcPixel[2] << 16;
                }
                break;

            case 4:
                truePixel = srcPixel[0] | srcPixel[1] << 8 | srcPixel[2] << 16 | srcPixel[3] << 24;
                break;

            default:
            	fprintf(stderr, "Error: LoadSurface - Image BPP of %d unsupported\n", bpp);
                free(raw);
                return false;
                break;
            }
            // Changing pixel order from RGBA to BGRA.
            SDL_GetRGBA( truePixel, data->format, &(dstPixel[2]), &(dstPixel[1]), &(dstPixel[0]), &(dstPixel[3]));
            dstPixel++;
            dstPixel++;
            dstPixel++;
            dstPixel++;
        }
    }
    SDL_UnlockSurface( data );
    *texture_width_ = data->w;
    *texture_height_ = data->h;

    while ( glGetError() ) { ; }

    GLuint texture;
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    *texture_ = static_cast<uint32>(texture);
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

    GLenum errorCode = glGetError();
    if ( errorCode != 0 ) {
        if ( errorCode == GL_OUT_OF_MEMORY )
            fprintf(stderr, "Error: LoadSurface - Out of texture memory!\n");
        else
        	fprintf(stderr, "Error: LoadSurface - Unknown error!\n");
        free(raw);
        return false;
    }
    // Many VGA works faster if you use BGR instead of RGB
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, *texture_width_, *texture_height_, 0, GL_BGRA, GL_UNSIGNED_BYTE, raw);
    free(raw);

    errorCode = glGetError();
    if ( errorCode != 0 ) {
        if ( errorCode == GL_OUT_OF_MEMORY )
            fprintf(stderr, "Error: LoadSurface - Out of texture memory!\n");
        else
        	fprintf(stderr, "Error: LoadSurface - Unknown error!\n");
        return false;
    }
    return true;
}

bool FlexibleSpritesheet::LoadFromFile(const std::string& filepath) {
    SDL_Surface* data = IMG_Load(filepath.c_str());
    bool result;
    if(data == NULL) {
        fprintf(stderr, "Could not load file \"%s\".\n", filepath.c_str());
        result = false;
    } else {
        
        printf("Hey my texture_ is %d, ", texture_);
        result = ConvertSurfaceToTexture(data, &texture_, &texture_width_, &texture_height_);
        printf("Now it's %d!\n", texture_);
        SDL_FreeSurface(data);

        frame_size_ = Vector2D(1.0f, 1.0f);
    }
    return result;
}

}  // namespace framework
