#include <ugdk/system/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/input/events.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/drawable/label.h>
#include <ugdk/graphic/drawable/textbox.h>
#include <ugdk/util/utf8.h>

#include <fstream>
#include <sstream>
#include <string>
#include <cerrno>
#include <memory>

using namespace ugdk;

static std::string get_file_contents(const std::string& filename) {
    std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
    if (in) {
        std::ostringstream contents;
        contents << in.rdbuf();
        in.close();
        return(contents.str());
    }
    throw(errno);
}

int main(int argc, char* argv[]) {
    system::Configuration config;
    // EXAMPLE_LOCATION is defined by CMake to be the full path to the directory
    // that contains the source code for this example
    config.base_path = EXAMPLE_LOCATION "/content/"; 
    system::Initialize(config);

    system::text_manager()->AddFont("default", "epgyosho.ttf", 30);
    { // scope so these objects are destroyed before system::Release
        graphic::Label label1(get_file_contents(system::ResolvePath("hello.txt")),
                                system::text_manager()->GetFont("default"));
        auto box = std::unique_ptr<graphic::TextBox>(system::text_manager()->GetTextFromFile("touhou.txt", "default"));

        action::Scene* scene = new action::Scene;
        scene->event_handler().AddListener<input::KeyPressedEvent>(
            [scene](const input::KeyPressedEvent& ev) {
                if(ev.scancode == input::Scancode::ESCAPE)
                    scene->Finish();
            });
        scene->set_render_function([&](graphic::Canvas& canvas) {
            label1.Draw(canvas);
            canvas.PushAndCompose(graphic::Geometry(math::Vector2D(0, label1.height() + 50)));
            box->Draw(canvas);
            canvas.PopGeometry();
        });

        system::PushScene(scene);
        system::Run();
    }
    system::Release();
    return 0;
}
