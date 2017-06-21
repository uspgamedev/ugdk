#include <ugdk/time/timeaccumulator.h>

#include "SDL.h"
#include "SDL_timer.h"

#include <ugdk/time/module.h>

namespace ugdk {
namespace time {

TimeAccumulator::TimeAccumulator(int duration_miliseconds) {
    Restart(duration_miliseconds);
}

int TimeAccumulator::TimeLeft() {
    return duration_ - (manager().TimeSince(initial_time_) - time_paused_);
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
    initial_time_ = manager().TimeElapsed();
    duration_ = (uint32) duration;
    time_paused_ = when_paused_ = 0;
}

void TimeAccumulator::Pause() {
    if(when_paused_ == 0)
        when_paused_ = manager().TimeElapsed();
}

void TimeAccumulator::Resume() {
    if(when_paused_ > 0) {
        time_paused_ += manager().TimeSince(when_paused_);
        when_paused_ = 0;
    }
}

bool TimeAccumulator::IsPaused() {
    return when_paused_ > 0;
}

}  // namespace time
}  // namespace ugdk
