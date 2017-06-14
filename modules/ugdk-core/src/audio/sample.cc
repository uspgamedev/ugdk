#include <algorithm>
#include "SDL.h"

#include <ugdk/audio/sample.h>
#include <ugdk/system/exceptions.h>
#include <ugdk/filesystem/module.h>
#include <ugdk/filesystem/file.h>
#include <filesystem/sdlfile.h>

namespace ugdk {
namespace audio {

Sample::Sample(const std::string& filepath)
    : data_(nullptr)
    , channel_(-1)
    , volume_(1.0)
{

    auto file = ugdk::filesystem::manager().OpenFile(filepath);

    SDL_RWops* rwops;
    if (auto ptr = dynamic_cast<filesystem::SDLFile*>(file.get())) {
        rwops = ptr->rwops();
    } else {
        throw system::BaseException("NYI: Sample from non-SDLFile.");
    }
    data_ = Mix_LoadWAV_RW(rwops, 0);
}

Sample::~Sample() {
    if(data_) {
        Mix_FreeChunk(data_);
        data_ = nullptr;
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
