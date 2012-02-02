#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>

#ifdef ISMAC
    #include "SDL.h"
#else
    #include <SDL/SDL.h>
#endif

#include <ugdk/base/types.h>
#include <ugdk/base/engine.h>
#include <ugdk/util/pathmanager.h>
#include <ugdk/graphic/spritesheet/fixedspritesheet.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/videomanager.h>

namespace ugdk {

FixedSpritesheetData::FixedSpritesheetData(const std::string& filename) {
    std::string filepath = PATH_MANAGER()->ResolvePath(filename);
    file_data_ = IMG_Load(filepath.c_str());
#ifdef DEBUG
    fprintf(stderr, "FixedSpritesheetData - NULL receive when loading \"%s\"\n", filepath.c_str());
#endif
}

FixedSpritesheetData::~FixedSpritesheetData() {
    if(file_data_ != NULL) {
        SDL_FreeSurface(file_data_);
    }

    for(std::vector<SpritesheetFrame>::iterator it = frames_.begin();
        it != frames_.end(); ++it)
        SDL_FreeSurface(it->surface);
}

void FixedSpritesheetData::AddFrame(int topleft_x, int topleft_y, int width, int height, const Vector2D& hotspot) {
    if(file_data_ == NULL) return;
    
    SDL_Surface* surface = NULL;
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, rmask, gmask, bmask, amask);

    SDL_LockSurface(file_data_);
    SDL_LockSurface(surface);

    Uint32 *source_pixels = static_cast<Uint32*>(file_data_->pixels);
    Uint32 *target_pixels = static_cast<Uint32*>(surface->pixels);
    
    for(int y = 0; y < height; ++y)
        for(int x = 0; x < width; ++x) {
            Uint8 r, g, b, a;
            SDL_GetRGBA(source_pixels[(y + topleft_y) * file_data_->w + (x + topleft_x)], file_data_->format, &r, &g, &b, &a);
            target_pixels[y * width + x] = SDL_MapRGBA(surface->format, r, g, b, a);
        }
    
    SDL_UnlockSurface(surface);
    SDL_UnlockSurface(file_data_);

    frames_.push_back(SpritesheetFrame(surface, hotspot));
}

void FixedSpritesheetData::FillWithFramesize(int width, int height, const Vector2D& hotspot) {
    if(file_data_ == NULL) return;
    for(int y = 0; y + height <= file_data_->h; y += height) {
        for(int x = 0; x + width <= file_data_->w; x += width) {
            AddFrame(x, y, width, height, hotspot);
        }
    }
}

FixedSpritesheet::FixedSpritesheet(FixedSpritesheetData& data) {
    const std::vector<FixedSpritesheetData::SpritesheetFrame>& frames = data.frames();

    lists_base_ = glGenLists(frames.size());

    std::vector<FixedSpritesheetData::SpritesheetFrame>::const_iterator it;
    GLuint id;
    for(it = frames.begin(), id = 0; it != frames.end(); ++it, ++id) {
        Texture* texture = Texture::CreateFromSurface(it->surface);
        CreateList(id, texture, it->hotspot);
        frames_.push_back(texture);
        frame_sizes_.push_back(Vector2D(static_cast<float>(texture->width()), static_cast<float>(texture->height())));
    }
}

FixedSpritesheet::~FixedSpritesheet() {
    glDeleteLists(lists_base_, frames_.size());

    // Clear the Textures
    for(std::vector<Texture*>::iterator it = frames_.begin();
        it != frames_.end(); ++it)
        delete *it;
}

void FixedSpritesheet::CreateList(GLuint id, Texture* texture, const Vector2D& hotspot) {
    if(texture == NULL) return;
    glColor3f(1.0f, 1.0f, 1.0f);

    Vector2D origin, target(static_cast<float>(texture->width()), static_cast<float>(texture->height()));
    origin -= hotspot;
    target -= hotspot;

    // Start the list
    glNewList(lists_base_ + id, GL_COMPILE); {
        // Enable textures then attach our texture
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture->gltexture());

        glBegin( GL_QUADS ); //Start quad
            //Draw square
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(  origin.x, origin.y );

            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(  target.x, origin.y );

            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(  target.x, target.y );

            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(  origin.x, target.y );
        glEnd();

    } glEndList();
}

void FixedSpritesheet::Draw(int frame_number, const Vector2D& hotspot) {
    const Modifier& mod = VIDEO_MANAGER()->CurrentModifier();
    if(!mod.visible()) return;

    bool popmatrix = false;
    if(mod.mirror() != MIRROR_NONE || hotspot.LengthSquared() > 1.0e-6) {
        glPushMatrix();
        // TODO: optimize mirroring, and combine the matrices

        // hotspot
        glTranslatef(-hotspot.x, -hotspot.y, 0.0f);

        // horizontal flip
        if(mod.mirror() & MIRROR_HFLIP) {
            glScalef(-1.0f, 1.0f, 1.0f);
        }

        // vertical flip
        if(mod.mirror() & MIRROR_VFLIP) {
            glScalef(1.0f, -1.0f, 1.0f);
        }
        popmatrix = true;
    }
    glColor4fv(mod.color().val);

    glCallList(lists_base_ + frame_number);

    if(popmatrix) glPopMatrix();
}

}  // namespace framework
