#include <ugdk/audio/manager.h>

#include "SDL.h"
#include "SDL_mixer.h"

#include <ugdk/system/engine.h>
#include <ugdk/debug/log.h>

#include <ugdk/audio/source.h>
#include <ugdk/audio/sampler.h>
#include <ugdk/audio/sampleframe.h>

#include <iostream>

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

    device_ = alcOpenDevice(NULL);
    if (device_) {
        ALCcontext *context = alcCreateContext(device_, NULL);
        alcMakeContextCurrent(context);
    }
    else
        return ErrorLog("Failed to initialize audio device");

    alListenerf(AL_GAIN, 1);
    alListener3f(AL_POSITION, 0, 0, 0);

    // sucesso ;)
    return true;
}

void Manager::Release() {
    alcCloseDevice(device_);
}

void Manager::Update() {
    for (auto& it : source_data_)
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

std::shared_ptr<Source> Manager::LoadSource(const std::string& name) {
    if (source_data_.find(name) == source_data_.end()) {
        std::shared_ptr<Source> source(new Source());
        if (source)
            source_data_[name] = source;
    }
    return source_data_[name];
}

} // namespace audio
} // namespace ugdk
