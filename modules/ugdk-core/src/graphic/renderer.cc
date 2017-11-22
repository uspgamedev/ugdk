
#include <cassert>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/modules.h>
#include <ugdk/graphic/renderer.h>
#include <ugdk/graphic/rendertarget.h>

namespace ugdk {
namespace graphic {

void Renderer::AssignTo(std::weak_ptr<RenderTarget> target) {
    target_ = target;
}

bool Renderer::IsValid() {
    return (target_.lock() && !(render_steps_.size()) );
}

void Renderer::AddStep(const std::function<void(graphic::Canvas&)>& render_step) {
    render_steps_.emplace_back(render_step);
}

void Renderer::ChangeStep(uint32_t index, 
                          const std::function<void(graphic::Canvas&)>& render_step) {
    assert( 0 <= index && index < render_steps_.size() )
    render_steps_[index] = render_step;
}

uint32_t Renderer::num_steps() {
    return render_steps_.size();
}

void Renderer::Process() {
    auto target = target_.lock();
    system::AssertCondition<system::InvalidOperation>(
        target,
        "Cannot Renderer::Process() when bound to nullptr");
    graphic::Canvas canvas(target.get());
    auto graphic_man = graphic::manager();

    graphic_man.UseCanvas(canvas);
    for (auto step : render_steps_) {
        step(canvas);
    }
    graphic_man.FreeCanvas(canvas);    
}

void Renderer::Present() {
    auto target = target_.lock().get();
    system::AssertCondition<system::InvalidOperation>(target,
                                                      "Cannot Renderer::Present() nullptr.");
    target->UpdateViewport();
}

} //graphic
} //ugdk