#include <ugdk/audio/manager.h>

#include "SDL.h"
#include "SDL_mixer.h"

#include <ugdk/system/engine.h>
#include <ugdk/audio/sample.h>
#include <ugdk/audio/music.h>
#include <ugdk/debug/log.h>

namespace ugdk {
namespace audio {

namespace {

bool ErrorLog(const std::string& err_msg) {
    debug::Log(debug::CRITICAL, err_msg);
    return false;
}
}

Manager::Manager() {
}

Manager::~Manager() {
}

bool Manager::Initialize() {

    if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
        return ErrorLog("Failed to initialize SDL_AUDIO: " + std::string(SDL_GetError()));

    // inicializa SDL_mixer
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) != 0)
        return ErrorLog("Failed to initialize SDL_Mixer: " + std::string(Mix_GetError()));

    // aloca canais de audio
    Mix_AllocateChannels(NUM_CHANNELS);
    for(int i=0; i<NUM_CHANNELS; i++)
        Mix_Volume(i, MIX_MAX_VOLUME);

    // sucesso ;)
    return true;
}

void Manager::Release() {
    ReleaseSamples();
    ReleaseMusics();
    Mix_CloseAudio();
}

void Manager::Update() {
}

void Manager::ReleaseSamples() {
    for(auto& it : sample_data_)
        it.second.reset();
}

void Manager::ReleaseMusics() {
    for(auto& it : music_data_)
        it.second.reset();
}

std::shared_ptr<Sample> Manager::LoadSample(const std::string& filepath) {
    if(sample_data_.find(filepath) == sample_data_.end()) {
        std::shared_ptr<Sample> sample ( new Sample(filepath));
        if(sample)
            sample_data_[filepath] = sample;
    }

    return sample_data_[filepath];
}

std::shared_ptr<Music> Manager::LoadMusic(const std::string& filepath) {
    if(music_data_.find(filepath) == music_data_.end()) {
        std::shared_ptr<Music> music ( new Music(filepath));
        if(music)
            music_data_[filepath] = music;
    }

    return music_data_[filepath];
}

std::shared_ptr<Music> Manager::CurrentMusic() const {
    return Music::playing_music_;
}

} // namespace audio
} // namespace ugdk
