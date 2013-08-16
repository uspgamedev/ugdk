#ifndef UGDK_GRAPHIC_PIXELSURFACE_H_
#define UGDK_GRAPHIC_PIXELSURFACE_H_

#define NO_SDL_GLEXT
#include <string>
#include "SDL.h"
#include "SDL_image.h"

namespace ugdk {
namespace graphic {


struct PixelSurface {
    SDL_Surface* surface;
    
    PixelSurface(SDL_Surface* _surface) : surface(_surface) {}
    PixelSurface(const std::string& filepath) : surface(IMG_Load(filepath.c_str())) {}
    ~PixelSurface() { if(surface) SDL_FreeSurface(surface); }
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_PIXELSURFACE_H_