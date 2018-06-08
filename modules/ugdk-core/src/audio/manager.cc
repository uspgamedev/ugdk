
#include <ugdk/audio/manager.h>

#include <ugdk/system/engine.h>
#include <ugdk/debug/log.h>

#include <ugdk/audio/sampler.h>
#include <ugdk/audio/sampleframe.h>

#include "SDL.h"
#include "SDL_mixer.h"
#include "openal/openal.h"

#include <iostream>

namespace ugdk {
namespace audio {

namespace {

bool ErrorLog(const std::string& err_msg) {
    debug::Log(debug::CRITICAL, err_msg);
    return false;
}

} // unnamed namespace

struct Manager::Backend final {
  ALCdevice *device{nullptr};
  ALCcontext *context{nullptr};
};

Manager::Manager() : backend_{} {}

Manager::~Manager() {}

bool Manager::Initialize() {

    backend_->device = alcOpenDevice(NULL);
    if (backend_->device) {
        backend_->context = alcCreateContext(backend_->device, NULL);
        alcMakeContextCurrent(backend_->context);
    }
    else
        return ErrorLog("Failed to initialize audio device");

    alListenerf(AL_GAIN, 1);
    alListener3f(AL_POSITION, 0, 0, 0);

    // sucesso ;)
    return true;
}

void Manager::Release() {
    alcCloseDevice(backend_->device);
}

void Manager::Update() {
    for (auto& it : track_data_)
        it.second->Update();
}

std::shared_ptr<Sampler> Manager::LoadSampler(const std::string& name, U64 sample_rate, U8 num_channels,
                                              const Sampler::Processor& processor) {
    if (sampler_data_.find(name) == sampler_data_.end()) {
        std::shared_ptr<Sampler> sampler(new Sampler(sample_rate, num_channels, processor));
        sampler_data_[name] = sampler;
    }
    return sampler_data_[name];
}

std::shared_ptr<Track> Manager::LoadTrack(const std::string& name) {
    if (track_data_.find(name) == track_data_.end()) {
        std::shared_ptr<Track> track{new Track()};
        if (track)
            track_data_[name] = track;
    }
    return track_data_[name];
}

} // namespace audio
} // namespace ugdk
