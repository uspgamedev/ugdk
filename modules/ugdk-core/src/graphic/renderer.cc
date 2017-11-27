
#include <cassert>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/renderer.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/exceptions.h>


namespace ugdk {
namespace graphic {

void Renderer::AssignTo(RenderTarget* target) {
    target_ = target;
}

bool Renderer::IsValid() {
    return (target_&& !(render_steps_.size()) );
}

void Renderer::AddStep(const std::function<void(graphic::Canvas&)>& render_step) {
    render_steps_.emplace_back(render_step);
}

void Renderer::ChangeStep(uint32_t index, 
                          const std::function<void(graphic::Canvas&)>& render_step) {
    system::AssertCondition<system::InvalidOperation>(0 <= index && index < render_steps_.size(),
                                                      "Cannot change a step beyond the size");
    render_steps_[index] = render_step;
}

uint32_t Renderer::num_steps() {
    return render_steps_.size();
}

void Renderer::Process() {
    system::AssertCondition<system::InvalidOperation>(
        target_,
        "Cannot Renderer::Process() when bound to nullptr");
        
    system::AssertCondition<system::InvalidOperation>(
        target_->IsValid(),
        "Cannot Renderer::Process() when bound to invalid");

    if (!target_->IsActive())
        target_->Bind();

    graphic::Canvas canvas(target_);
    canvas.Bind(); //We set the shaderprogram
    target_->Use(); //We mess with the context
    for (auto step : render_steps_) {
        step(canvas);
    }

    target_->Unbind();
    canvas.Unbind(); // restore shaderprogram
}

void Renderer::Present() {
    system::AssertCondition<system::InvalidOperation>(target_,
                                                      "Cannot Renderer::Present() nullptr.");    
    target_->UpdateViewport();
    target_->Present();
}

} //graphic
} //ugdk