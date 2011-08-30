#include <SDL/SDL.h>
#include <SDL/SDL_timer.h>
#include <ugdk/time/timehandler.h>

namespace ugdk {

TimeHandler::TimeHandler() {
    last_update_ = current_time_ = initial_time_ = SDL_GetTicks();
    paused_ = false;
    time_paused_ = 0;
}

void TimeHandler::Update() {
    last_update_ = current_time_;
    current_time_ = SDL_GetTicks();
    if( paused_ )
        time_paused_ += current_time_ - last_update_;
}

uint32 TimeHandler::TimeElapsed() {
    return current_time_ - initial_time_; //TimeSince(initial_time_);
}

uint32 TimeHandler::TimeDifference() {
    return current_time_ - last_update_;
}

uint32 TimeHandler::TimeSince(uint32 t0) {
    return current_time_ - t0 - time_paused_ - initial_time_;
}

void TimeHandler::Pause() {
    paused_ = true;
}

void TimeHandler::Resume() {
    paused_ = false;
}

bool TimeHandler::IsPaused() {
    return paused_;
}

}  // namespace framework
