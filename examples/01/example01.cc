#include <ugdk/system/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/input/events.h>

int main(int argc, char* argv[]) {
    assert(ugdk::system::Initialize());
    ugdk::action::Scene* scene = new ugdk::action::Scene;
    scene->event_handler().AddListener<ugdk::input::KeyPressedEvent>(
        [scene](const ugdk::input::KeyPressedEvent& ev) {
            if(ev.scancode == ugdk::input::Scancode::ESCAPE)
                scene->Finish();
        });
    ugdk::system::PushScene(scene);
    ugdk::system::Run();
    ugdk::system::Release();
    return 0;
}
