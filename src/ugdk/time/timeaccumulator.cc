#include <ugdk/config/config.h>
#ifdef ISMAC
    #include "SDL.h"
    #include "SDL_timer.h"
#else
    #include <SDL/SDL.h>
    #include <SDL/SDL_timer.h>
#endif
#include <ugdk/time/timehandler.h>
#include <ugdk/time/timeaccumulator.h>
#include <ugdk/base/engine.h>

namespace ugdk {

TimeAccumulator::TimeAccumulator(int duration_miliseconds) {
    handler_ = Engine::reference()->time_handler();
    Restart(duration_miliseconds);
}

int TimeAccumulator::TimeLeft() {
    return duration_ - (handler_->TimeSince(initial_time_) - time_paused_);
}

bool TimeAccumulator::Expired() {
	if(when_paused_ == 0)
		return TimeLeft() <= 0;
	return 0;
}

void TimeAccumulator::Restart() {
    Restart(duration_);
}

void TimeAccumulator::Restart(int duration) {
    initial_time_ = handler_->TimeElapsed();
    duration_ = (uint32) duration;
    time_paused_ = when_paused_ = 0;
}

void TimeAccumulator::Pause() {
    if(when_paused_ == 0)
        when_paused_ = handler_->TimeElapsed();
}

void TimeAccumulator::Resume() {
    if(when_paused_ > 0) {
        time_paused_ += handler_->TimeSince(when_paused_);
        when_paused_ = 0;
    }
}

bool TimeAccumulator::IsPaused() {
    return when_paused_ > 0;
}

}

