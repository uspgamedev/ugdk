#include <ugdk/audio/manager.h>

#include "SDL.h"
#include "SDL_mixer.h"

#include <ugdk/util/pathmanager.h>
#include <ugdk/system/engine.h>
#include <ugdk/audio/sample.h>
#include <ugdk/audio/music.h>

namespace ugdk {
namespace audio {

Manager::Manager() {
}

Manager::~Manager() {
}

bool Manager::Initialize() {
    // inicializa SDL_mixer
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) != 0)
        return false;

    // aloca canais de audio
    Mix_AllocateChannels(NUM_CHANNELS);
    for(int i=0; i<NUM_CHANNELS; i++)
        Mix_Volume(i, MIX_MAX_VOLUME);

    // sucesso ;)
    return true;
}

bool Manager::Release() {
    ReleaseSamples();
    ReleaseMusics();
    Mix_CloseAudio();

    // sucesso ;)
    return true;
}

void Manager::Update() {
}

void Manager::ReleaseSamples() {
    std::map<std::string, Sample*>::iterator it;
    for(it = sample_data_.begin(); it != sample_data_.end(); ++it)
        delete it->second;
}

void Manager::ReleaseMusics() {
    std::map<std::string, Music*>::iterator it;
    for(it = music_data_.begin(); it != music_data_.end(); ++it)
        delete it->second;
}

Sample* Manager::LoadSample(const std::string& filepath) {
	std::string fullpath = PATH_MANAGER()->ResolvePath(filepath);
    if(sample_data_.find(filepath) == sample_data_.end()) {
        Sample *sample = new Sample(fullpath);
        if(sample)
            sample_data_[filepath] = sample;
    }

    return sample_data_[filepath];
}

Music* Manager::LoadMusic(const std::string& filepath) {
	std::string fullpath = PATH_MANAGER()->ResolvePath(filepath);
    if(music_data_.find(filepath) == music_data_.end()) {
        Music *music = new Music(fullpath);
        if(music)
            music_data_[filepath] = music;
    }

    return music_data_[filepath];
}

Music* Manager::CurrentMusic() const {
    return Music::playing_music_;
}

} // namespace audio
} // namespace ugdk

