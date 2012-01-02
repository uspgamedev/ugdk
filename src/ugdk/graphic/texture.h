#ifndef UGDK_GRAPHIC_TEXTURE_H_
#define UGDK_GRAPHIC_TEXTURE_H_

#include <SDL/SDL_video.h>
#include <SDL/SDL_opengl.h>

namespace ugdk {

class Texture {
  public:
    ~Texture();
    static Texture* CreateFromSurface(SDL_Surface* data);

    GLuint gltexture() const { return gltexture_; }
    const int width() const { return texture_width_; }
    const int height() const { return texture_height_; }

  private:
    Texture(GLuint gltexture, int texture_width, int texture_height);

    GLuint gltexture_;
    int texture_width_, texture_height_;
};

}  // namespace framework

#endif
