#ifndef UGDK_GRAPHIC_RENDERSCREEN_H_
#define UGDK_GRAPHIC_RENDERSCREEN_H_

#include <memory>
#include <ugdk/graphic/rendertarget.h>

namespace ugdk {

namespace math {
class Vector2D;
}

namespace desktop {
class Window;
}

namespace graphic {
class GLTexture;

class RenderScreen : public RenderTarget {
  public:
    void Resize(const math::Vector2D&);
    void SaveToTexture(graphic::GLTexture*);
    void AttachTo(const std::weak_ptr<desktop::Window>&);

  private:
    std::weak_ptr<desktop::Window> window_;
    math::Vector2D size_;
};

}
}

#endif