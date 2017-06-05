#include <ugdk/action/mediaplayer.h>

#include <ugdk/action/mediamanager.h>

namespace ugdk {
namespace action {

MediaPlayer::MediaPlayer()
    : manager_(nullptr)
{}

MediaPlayer::~MediaPlayer() {
    if(manager_)
        manager_->RemovePlayer(this);
}

void MediaPlayer::AddTickFunction(std::function<void (void)> tick) {
    ticks_.push_back(tick);
}

void MediaPlayer::ChangeMediaManager(MediaManager* manager) {
    manager->AddPlayer(this); // This function changes mangaer_
}

void MediaPlayer::notifyAllObservers() {
    for (const auto& tick : ticks_) {
        tick();
    }
}

} /* namespace action*/
} /* namespace ugdk */




