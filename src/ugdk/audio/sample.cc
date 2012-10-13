#include <algorithm>
#include "SDL.h"
#include <ugdk/audio/sample.h>

namespace ugdk {
namespace audio {

Sample::Sample(const std::string& filepath) : data_(NULL), channel_(-1), volume_(1.0) {
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

void Sample::PlayForever() {
    Play(-1);
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

void Sample::SetVolume(double vol) {
    volume_ = std::min(1.0, std::max(0.0, vol));
    if(data_)
        Mix_VolumeChunk(data_, int( volume_ * double(MIX_MAX_VOLUME) ));
}

double Sample::Volume() {
    return volume_;
}

} // namespace audio
} // namespace ugdk
