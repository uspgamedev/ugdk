#ifndef UGDK_INTERNAL_PIXELSURFACE_H_
#define UGDK_INTERNAL_PIXELSURFACE_H_

#include <string>
#include "SDL_surface.h"

namespace ugdk {
namespace internal {

struct PixelSurface {
    SDL_Surface* surface;
    
    PixelSurface(SDL_Surface* _surface);
    PixelSurface(const std::string& filepath);
    ~PixelSurface();
};

}  // namespace internal
}  // namespace ugdk

#endif // UGDK_INTERNAL_PIXELSURFACE_H_
