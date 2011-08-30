#include <algorithm>
#include <ugdk/audio/sample.h>
namespace ugdk {

Sample::Sample(const std::string& filepath) : data_(NULL), channel_(-1), volume_(1.0f) {
    data_ = Mix_LoadWAV(filepath.c_str());
}

Sample::~Sample() {
    if(data_) {
        Mix_FreeChunk(data_);
        data_ = NULL;
    }
}

void Sample::Play() {
    Play(0);
}

void Sample::Play(int loops) {
    if(data_)
        channel_ = Mix_PlayChannel(-1, data_, loops);
}

bool Sample::IsPlaying() {
    return Mix_Playing(channel_) != 0;
}

void Sample::Stop() {
    if(IsPlaying()) {
        Mix_HaltChannel(channel_);
        channel_ = -1;
    }
}

void Sample::SetVolume(float vol) {
    volume_ = std::min(1.0f, std::max(0.0f, vol));
    if(data_)
        Mix_VolumeChunk(data_, int( volume_ * float(MIX_MAX_VOLUME) ));
}

float Sample::Volume() {
    return volume_;
}

}  // namespace framework
