#include <ugdk/graphic/spritesheetdata.h>

#include <ugdk/internal/pixelsurface.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/system/engine.h>

#include <GL/glew.h>
#include "SDL_image.h"

#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>

namespace ugdk {
namespace graphic {

static SDL_Surface* createBaseSurface(Uint32 width, Uint32 height) {
    SDL_Surface* surface = nullptr;
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
    return surface;
}

static void extractPartOfSurface(SDL_Surface* source, SDL_Surface* target, int topleft_x, int topleft_y, int width, int height) {
    SDL_LockSurface(source);
    SDL_LockSurface(target);

    Uint32 *source_pixels = static_cast<Uint32*>(source->pixels);
    Uint32 *target_pixels = static_cast<Uint32*>(target->pixels);
    
    for(int y = 0; y < height; ++y)
        for(int x = 0; x < width; ++x) {
            Uint8 r, g, b, a;
            SDL_GetRGBA(source_pixels[(y + topleft_y) * source->w + (x + topleft_x)], source->format, &r, &g, &b, &a);
            target_pixels[y * width + x] = SDL_MapRGBA(target->format, r, g, b, a);
        }
    
    SDL_UnlockSurface(target);
    SDL_UnlockSurface(source);
}

SpritesheetData::SpritesheetFrame::~SpritesheetFrame() {
    delete surface;
}

SpritesheetData::SpritesheetData(const std::string& filename) {
    std::string filepath = ugdk::system::ResolvePath(filename);
    file_data_.push_back(new internal::PixelSurface(filepath));
#ifdef DEBUG
    if(file_data_.back()->surface == nullptr)
        fprintf(stderr, "SpritesheetData - nullptr received when loading \"%s\"\n", filepath.c_str());
#endif
}

SpritesheetData::SpritesheetData(const std::list<std::string>& filenames) {
    std::list<std::string>::const_iterator it;
    for(it = filenames.begin(); it != filenames.end(); ++it) {
        std::string filepath = ugdk::system::ResolvePath(*it);
        file_data_.push_back(new internal::PixelSurface(filepath));
#ifdef DEBUG
        if(file_data_.back()->surface == nullptr)
            fprintf(stderr, "SpritesheetData - nullptr received when loading \"%s\"\n", filepath.c_str());
#endif
    }
}

SpritesheetData::~SpritesheetData() {
    for(auto& file_surface : file_data_)
        delete file_surface;
}

void SpritesheetData::AddFrame(int topleft_x, int topleft_y, int width, int height, const ugdk::math::Vector2D& hotspot, size_t file) {
    if(file >= file_data_.size() || file_data_[file]->surface == nullptr) return;
    
    SDL_Surface* surface = createBaseSurface(width, height);
    extractPartOfSurface(file_data_[file]->surface, surface, topleft_x, topleft_y, width, height);

    frames_.emplace_back(new internal::PixelSurface(surface), hotspot);
}

void SpritesheetData::FillWithFramesize(int width, int height, const ugdk::math::Vector2D& hotspot, size_t file) {
    if(file >= file_data_.size() || file_data_[file]->surface == nullptr) return;

    for(int y = 0; y + height <= file_data_[file]->surface->h; y += height) {
        for(int x = 0; x + width <= file_data_[file]->surface->w; x += width) {
            AddFrame(x, y, width, height, hotspot);
        }
    }
}
    
void SpritesheetData::FillWithFramesizeFromAllFiles(int width, int height, const ugdk::math::Vector2D& hotspot) {
    for(size_t i = 0; i < file_data_.size(); ++i)
        FillWithFramesize(width, height, hotspot, i);
}

}  // namespace graphic
}  // namespace ugdk
