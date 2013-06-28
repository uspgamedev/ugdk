#include <ugdk/system/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/input/module.h>

int main(int argc, char* argv[]) {
    ugdk::system::Initialize();
    ugdk::action::Scene* scene = new ugdk::action::Scene();
    scene->AddTask([scene](double dt) {
        if(ugdk::input::manager()->KeyDown(ugdk::input::K_ESCAPE))
            scene->Finish();
        return true; // Keep the task alive
    });
    ugdk::system::PushScene(scene);
    ugdk::system::Run();
    ugdk::system::Release();
    return 0;
}