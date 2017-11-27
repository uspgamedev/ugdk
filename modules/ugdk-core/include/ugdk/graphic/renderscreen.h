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
    RenderScreen();
    RenderScreen(const std::weak_ptr<desktop::Window>&);

    void SaveToTexture(graphic::GLTexture*);
    void AttachTo(const std::weak_ptr<desktop::Window>&);

    bool IsValid() const override;

    std::weak_ptr<desktop::Window> Window() const;

    void Resize(const math::Vector2D &);
    math::Vector2D size() const override;
    void Use() override;
  private:
    void UpdateViewport() override;
    std::weak_ptr<desktop::Window> window_;
    math::Vector2D size_;
};

}
}

#endif