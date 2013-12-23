#include <ugdk/system/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/input/events.h>

int main(int argc, char* argv[]) {
    assert(ugdk::system::Initialize());
    ugdk::action::Scene* ourscene = new ugdk::action::Scene;
    ourscene->event_handler().AddListener<ugdk::input::KeyPressedEvent>(
        [ourscene](const ugdk::input::KeyPressedEvent& ev) {
            if(ev.scancode == ugdk::input::Scancode::ESCAPE)
                ourscene->Finish();
        });
    ugdk::system::PushScene(ourscene);
    ugdk::system::Run();
    ugdk::system::Release();
    return 0;
}
