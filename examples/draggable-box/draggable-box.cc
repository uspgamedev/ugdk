#include <ugdk/system/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/input/module.h>
#include <ugdk/input/events.h>
#include <ugdk/input/scancode.h>
#include <ugdk/action/entity.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/module.h>
#include <ugdk/system/config.h>

using namespace ugdk;

int main(int argc, char *argv[]) {

    system::Initialize();

    action::Scene* scene = new action::Scene();
    
    auto r = new graphic::TexturedRectangle(graphic::manager()->white_texture(), math::Vector2D(50.0, 50.0));
    scene->set_render_function([r](graphic::Canvas& canvas) {
        canvas.PushAndCompose(graphic::Geometry(math::Vector2D(300, 200)));
        r->Draw(canvas);
        canvas.PopGeometry();
    });

    system::PushScene(scene);
    system::Run();
    delete r;
    
    system::Release();
    return 0;
}