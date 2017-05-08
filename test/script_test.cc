#include <ugdk/system/config.h>
#include <cstdlib>
#include <ugdk/system/engine.h>

int main(int argc, char **argv) {
    ugdk::system::Configuration config;
    config.base_path = "data/";
    config.audio_enabled = false;
    config.input_enabled = false;
    config.windows_list.clear();

    ugdk::system::Initialize(config);

    ugdk::system::Run();
    ugdk::system::Release();
    return 0;
}
