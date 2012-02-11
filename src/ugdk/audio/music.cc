#include <algorithm>
#include <ugdk/audio/music.h>

namespace ugdk {

Music* Music::playing_music_(NULL);

Music::Music(const std::string& filepath) : data_(NULL), volume_(1.0) {
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
        playing_music_ = this;
        UpdateVolume(volume_);
    }
}

void Music::Stop() {
    if(playing_music_ == this) {
        Mix_HaltMusic();
        playing_music_ = NULL;
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
    playing_music_ = NULL;
}

void Music::UpdateVolume(double vol) {
    Mix_VolumeMusic( int( vol * double(MIX_MAX_VOLUME) ) );
}

}  // namespace ugdk

