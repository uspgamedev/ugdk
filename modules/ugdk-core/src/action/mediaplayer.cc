#include <ugdk/action/mediaplayer.h>

#include <ugdk/action/mediamanager.h>

namespace ugdk {
namespace action {

MediaPlayer::MediaPlayer()
    : manager_(nullptr)
{}

void MediaPlayer::AddTickFunction(std::function<void (void)> tick) {
    ticks_.push_back(tick);
}

void MediaPlayer::notifyAllObservers() {
    for (const auto& tick : ticks_) {
        tick();
    }
}

} /* namespace action*/
} /* namespace ugdk */




