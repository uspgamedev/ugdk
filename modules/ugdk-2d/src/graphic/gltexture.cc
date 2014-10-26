#include "gltexture.h"

#include "SDL_endian.h"
#include "SDL_image.h"

#include <ugdk/graphic/opengl.h>
#include <ugdk/system/engine.h>
#include <ugdk/system/config.h>
#include <ugdk/debug/log.h>

#include <cstdlib>
#include <cstdio>

namespace ugdk {
namespace graphic {

namespace {
    bool ConvertSurfaceToTexture(SDL_Surface* data, GLuint* texture_, int* texture_width_, int* texture_height_) {
        Uint8 *raw = static_cast<Uint8*>(malloc(data->w * data->h * 4));
        Uint8 *dstPixel = raw;

        SDL_LockSurface(data);
        int bpp = data->format->BytesPerPixel;
        Uint8 *srcPixel;
        Uint32 truePixel;

        for (int i = 0; i < data->h; i++) {
            for (int j = 0; j < data->w; j++) {
                srcPixel = (Uint8 *)data->pixels + i * data->pitch + j * bpp;
                switch (bpp) {
                    case 1:
                        truePixel = *srcPixel;
                        break;

                    case 2:
                        truePixel = *srcPixel;
                        break;

                    case 3:
                        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                            truePixel = srcPixel[0] << 16 | srcPixel[1] << 8 | srcPixel[2];
                        } else {
                            truePixel = srcPixel[0] | srcPixel[1] << 8 | srcPixel[2] << 16;
                        }
                        break;

                    case 4:
                        truePixel = srcPixel[0] | srcPixel[1] << 8 | srcPixel[2] << 16 | srcPixel[3] << 24;
                        break;

                    default:
                        debug::Log(debug::LogLevel::ERROR, "UGDK",
                                   "LoadSurface - Image BPP of ", bpp, " unsupported.");
                        free(raw);
                        return false;
                        break;
                }
                // Changing pixel order from RGBA to BGRA.
                SDL_GetRGBA(truePixel, data->format, &(dstPixel[0]), &(dstPixel[1]), &(dstPixel[2]), &(dstPixel[3]));
                dstPixel++;
                dstPixel++;
                dstPixel++;
                dstPixel++;
            }
        }
        SDL_UnlockSurface(data);
        *texture_width_ = data->w;
        *texture_height_ = data->h;

        // Clear previous errors so the next error checking works.
        internal::ClearOpenGLErrors();

        glGenTextures(1, texture_);
        glBindTexture(GL_TEXTURE_2D, *texture_);

        /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

#ifndef UGDK_USING_GLES
        glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
#endif

        GLenum errorCode = glGetError();
        if (errorCode != 0) {
            debug::Log(debug::LogLevel::ERROR, "UGDK",
                       "LoadSurface - ",
                       errorCode == GL_OUT_OF_MEMORY
                       ? "Out of texture memory"
                       : "Unknown error");
            free(raw);
            return false;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *texture_width_, *texture_height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, raw);
        free(raw);

        errorCode = glGetError();
        if (errorCode != 0) {
            debug::Log(debug::LogLevel::ERROR, "UGDK",
                       "LoadSurface - ",
                       errorCode == GL_OUT_OF_MEMORY
                       ? "Out of texture memory"
                       : "Unknown error");
            return false;
        }
        return true;
    }
}

GLTexture::~GLTexture() {
    glDeleteTextures(1, &id_);
}

GLTexture* GLTexture::CreateFromFile(const std::string& filepath) {
    std::string fullpath = ugdk::system::ResolvePath(filepath);
    SDL_Surface* data = IMG_Load(fullpath.c_str());

    if(data == nullptr) {
        debug::Log(debug::LogLevel::ERROR, "UGDK",
                   "Texture::CreateFromFile - error loading file '", fullpath, "' -> ", SDL_GetError());
        return nullptr;
    }
    GLTexture* tex = CreateFromSurface(data);
    SDL_FreeSurface(data);
    return tex;
}

GLTexture* GLTexture::CreateFromSurface(SDL_Surface* data) {
    if(data == nullptr) {
        debug::Log(debug::LogLevel::ERROR, "UGDK",
                   "Texture::CreateFromFile - No Data");
        return nullptr; // Safeguard from nullptr
    }

    GLuint gltex;
    int texture_width, texture_height;
    bool result = ConvertSurfaceToTexture(data, &gltex, &texture_width, &texture_height);
    if(!result) return nullptr;

    return new GLTexture(gltex, texture_width, texture_height);
}

GLTexture* GLTexture::CreateRawTexture(int texture_width, int texture_height) {
    GLuint texture;
    glGenTextures( 1, &texture );
    return new GLTexture(texture, texture_width, texture_height);
}

GLTexture::GLTexture(GLuint gltexture, int texture_width, int texture_height) 
    : id_(gltexture), texture_width_(texture_width), texture_height_(texture_height) {}

} // namespace graphic
} // namespace ugdk
