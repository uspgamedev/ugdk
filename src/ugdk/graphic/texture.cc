#include <SDL/SDL_endian.h>

#include <ugdk/graphic/texture.h>
#include <cstdlib>
#include <cstdio>

namespace ugdk {

Texture::~Texture() {
    glDeleteTextures(1, &gltexture_);
}

static bool ConvertSurfaceToTexture(SDL_Surface* data, GLuint* texture_, int* texture_width_, int* texture_height_) {
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

    glGenTextures( 1, texture_ );
    glBindTexture( GL_TEXTURE_2D, *texture_ );
    
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

Texture* Texture::CreateFromSurface(SDL_Surface* data) {
    if(data == NULL) {
        fprintf(stderr, "Error: CreateFromSurface - No Data\n");
        return NULL; // Safeguard from NULL
    }

    GLuint gltex;
    int texture_width, texture_height;
    bool result = ConvertSurfaceToTexture(data, &gltex, &texture_width, &texture_height);
    if(!result) return NULL;

    return new Texture(gltex, texture_width, texture_height);
}

Texture* Texture::CreateRawTexture(int texture_width, int texture_height) {
    GLuint texture;
    glGenTextures( 1, &texture );
    return new Texture(texture, texture_width, texture_height);
}

Texture::Texture(GLuint gltexture, int texture_width, int texture_height) 
    : gltexture_(gltexture), texture_width_(texture_width), texture_height_(texture_height) {}

}  // namespace ugdk
