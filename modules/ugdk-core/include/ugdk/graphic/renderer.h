#ifndef UGDK_GRAPHIC_RENDERER_H_
#define UGDK_GRAPHIC_RENDERER_H_

#include <functional>
#include <vector>
#include <memory>


namespace ugdk {
namespace graphic {

class Canvas;
class RenderTarget;

class Renderer {
  private:
    std::weak_ptr<RenderTarget> target_;
    std::vector<std::function<void(graphic::Canvas&)>> render_steps_;
  public:
    void AssignTo(std::weak_ptr<RenderTarget>);
    bool IsValid();

    /// Emplace a render step into the Renderer
    void AddStep(const std::function<void(graphic::Canvas&)>&);
    /// Update a given render_step
    void ChangeStep(uint32_t, const std::function<void(graphic::Canvas&)>&);
    /// Number of render_steps currently registered
    uint32_t num_steps();

    /* WE MIGHT WANT TO MAKE RENDER_STEPS BE ORDERED JUST BY PRIORITY */

    /// Processes through the render_steps 
    void Process();
    /// Presents the current canvas in the screen.
    void Present();
};

}
}

#endif
