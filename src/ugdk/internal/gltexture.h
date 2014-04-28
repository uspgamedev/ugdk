#ifndef UGDK_INTERNAL_TEXTURE_H_
#define UGDK_INTERNAL_TEXTURE_H_

#include <ugdk/internal/opengl.h>

#include <string>
#include "SDL_video.h"

namespace ugdk {
namespace internal {

class GLTexture {
  public:
    ~GLTexture();
    static GLTexture* CreateFromFile(const std::string& filepath);
    static GLTexture* CreateFromSurface(SDL_Surface* data);
    static GLTexture* CreateRawTexture(int texture_width, int texture_height);

    GLuint id() const { return id_; }
    int width() const { return texture_width_; }
    int height() const { return texture_height_; }

  private:
    GLTexture(GLuint gltexture, int texture_width, int texture_height);

    GLuint id_;
    int texture_width_, texture_height_;
};

}  // namespace internal
}  // namespace ugdk

#endif
