#include <algorithm>
#include "SDL.h"

#include <ugdk/audio/music.h>
#include <ugdk/system/exceptions.h>
#include <ugdk/filesystem/module.h>
#include <ugdk/filesystem/file.h>
#include <filesystem/sdlfile.h>

namespace ugdk {
namespace audio {

Music* Music::playing_music_(nullptr);

Music::Music(const std::string& filepath)
    : data_(nullptr)
    , volume_(1.0)
{
    auto file = ugdk::filesystem::manager()->OpenFile(filepath);

    SDL_RWops* rwops;
    if (auto ptr = dynamic_cast<filesystem::SDLFile*>(file.get())) {
        rwops = ptr->rwops();
    } else {
        throw system::BaseException("NYI: Sample from non-SDLFile.");
    }
    data_ = Mix_LoadMUS_RW(rwops, 0);
    Mix_HookMusicFinished(MusicDone);
}

Music::~Music() {
    if(data_) {
        Mix_FreeMusic(data_);
        data_ = nullptr;
    }
}

void Music::Play() {
    Play(0);
}

void Music::PlayForever() {
    Play(-1);
}

void Music::Play(int loops) {
    if(data_ && Mix_PlayMusic(data_, loops) == 0) {
        playing_music_ = this;
        UpdateVolume(volume_);
    }
}

void Music::Stop() {
    if(playing_music_ == this) {
        Mix_HaltMusic();
        playing_music_ = nullptr;
    }
}

bool Music::IsPlaying() const {
    return playing_music_ == this;
}

void Music::Pause() {
    if(playing_music_ == this)
        Mix_PauseMusic();
}

void Music::Unpause() {
    if(playing_music_ == this)
        Mix_ResumeMusic();
}

bool Music::IsPaused() const {
    return IsPlaying() && (Mix_PausedMusic() == 1);
}

void Music::SetVolume(double vol) {
    volume_ = std::min(1.0, std::max(0.0, vol));
    if(this->IsPlaying())
        UpdateVolume(volume_);
}

double Music::Volume() {
    return volume_;
}

void Music::MusicDone() {
    playing_music_ = nullptr;
}

void Music::UpdateVolume(double vol) {
    Mix_VolumeMusic( int( vol * double(MIX_MAX_VOLUME) ) );
}

} // namespace audio
} // namespace ugdk

