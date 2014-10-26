#include "pixelsurface.h"

#include "SDL_image.h"

namespace ugdk {
namespace internal {

PixelSurface::PixelSurface(SDL_Surface* _surface) 
    : surface(_surface) {}

PixelSurface::PixelSurface(const std::string& filepath) 
    : surface(IMG_Load(filepath.c_str())) {}

PixelSurface::~PixelSurface() { 
    if(surface)
        SDL_FreeSurface(surface);
}

}  // namespace internal
}  // namespace ugdk
