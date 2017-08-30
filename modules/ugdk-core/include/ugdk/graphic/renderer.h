#ifndef UGDK_GRAPHIC_RENDERER_H_
#define UGDK_GRAPHIC_RENDERER_H_

namespace ugdk {
namespace graphic {

class Renderer {
  private:
  public:
    /// Presents the current canvas in the screen.
    virtual void Present() = 0;
};

}
}

#endif
