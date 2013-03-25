#ifndef UGDK_GRAPHIC_TEXTURE_H_
#define UGDK_GRAPHIC_TEXTURE_H_

#include <string>
#include "SDL_opengl.h"
#include "SDL_video.h"

namespace ugdk {
namespace graphic {

class Texture {
  public:
    ~Texture();
    static Texture* CreateFromFile(const std::string& filepath);
    static Texture* CreateFromSurface(SDL_Surface* data);
    static Texture* CreateRawTexture(int texture_width, int texture_height);

    GLuint gltexture() const { return gltexture_; }
    int width() const { return texture_width_; }
    int height() const { return texture_height_; }

  private:
    Texture(GLuint gltexture, int texture_width, int texture_height);

    GLuint gltexture_;
    int texture_width_, texture_height_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
