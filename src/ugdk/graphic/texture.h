#ifndef UGDK_GRAPHIC_TEXTURE_H_
#define UGDK_GRAPHIC_TEXTURE_H_

#include <string>
#include <ugdk/config/config.h>
#include "SDL_video.h"
#include "SDL_opengl.h"

namespace ugdk {
namespace graphic {

class Texture {
  public:
    ~Texture();
    static Texture* CreateFromFile(const std::string& filepath);
    static Texture* CreateFromSurface(SDL_Surface* data);
    static Texture* CreateRawTexture(int texture_width, int texture_height);

    GLuint gltexture() const { return gltexture_; }
    const int width() const { return texture_width_; }
    const int height() const { return texture_height_; }

  private:
    Texture(GLuint gltexture, int texture_width, int texture_height);

    GLuint gltexture_;
    int texture_width_, texture_height_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
