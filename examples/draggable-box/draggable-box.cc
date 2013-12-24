#include <ugdk/system/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/input/events.h>
#include <ugdk/action/entity.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/module.h>
#include <ugdk/system/config.h>
#include <ugdk/desktop/module.h>
#include <ugdk/desktop/window.h>

using namespace ugdk;

int main(int argc, char *argv[]) {

    system::Initialize();

    math::Vector2D box_size(50.0, 50.0);

    auto window_size = desktop::manager()->primary_window()->size();
    auto canvas_size = graphic::manager()->canvas()->size();
    math::Vector2D window_to_canvas(canvas_size.x / window_size.x, canvas_size.y / window_size.y);
    graphic::Geometry box_position;

    action::Scene* scene = new action::Scene();
    
    auto r = new graphic::TexturedRectangle(graphic::manager()->white_texture(), box_size);
    r->set_hotspot(graphic::Drawable::CENTER);
    scene->set_render_function([&](graphic::Canvas& canvas) {
        canvas.PushAndCompose(box_position);
        r->Draw(canvas);
        canvas.PopGeometry();
    });
    scene->event_handler().AddListener<input::MouseMotionEvent>(
        [&](const input::MouseMotionEvent& ev) {
            box_position.ChangeOffset(math::Vector2D(ev.position).Scale(window_to_canvas));
        });

    system::PushScene(scene);
    system::Run();
    delete r;
    
    system::Release();
    return 0;
}