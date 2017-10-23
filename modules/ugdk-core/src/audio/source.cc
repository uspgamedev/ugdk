#include <ugdk/system/config.h>
#include <ugdk/audio.h>

#include "AL.h"

#include <queue>

namespace ugdk {
namespace audio {

Source::Source() {

}
Source::~Source() {

}
bool Source::QueueSampler(Sampler s) {

}
Sampler Source::UnqueueSampler() {

}
void Source::ClearQueue() {

}
void Source::Play() {
    alSourcePlay(_name);
}
void Source::Pause() {
    alSourcePause(_name);
}
void Source::Stop() {
    alSourceStop(_name);
}
void Source::Rewind() {
    actual_ = queue_[0];
}
bool Source::IsPlaying() const {

}
bool Source::IsPaused() const {

}
void Source::SetVolume(double vol) {

}
double Source::Volume() const {

}
