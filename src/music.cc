#include <algorithm>
#include "music.h"

namespace ugdk {

Mix_Music* Music::playing_music_(NULL);

Music::Music(const std::string& filepath) : data_(NULL), volume_(1.0f) {
    data_ = Mix_LoadMUS(filepath.c_str());
    Mix_HookMusicFinished(MusicDone);
}

Music::~Music() {
    if(data_) {
        Mix_FreeMusic(data_);
        data_ = NULL;
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
        playing_music_ = data_;
        UpdateVolume(volume_);
    }
}

void Music::Stop() {
    if(playing_music_ == data_) {
        Mix_HaltMusic();
        playing_music_ = NULL;
    }
}

bool Music::IsPlaying() {
    return playing_music_ == data_;
}

void Music::SetVolume(float vol) {
    volume_ = std::min(1.0f, std::max(0.0f, vol));
    if(this->IsPlaying())
        UpdateVolume(volume_);
}

float Music::Volume() {
    return volume_;
}

void Music::MusicDone() {
    playing_music_ = NULL;
}

void Music::UpdateVolume(float vol) {
    Mix_VolumeMusic( int( vol * float(MIX_MAX_VOLUME) ) );
}

}  // namespace framework

