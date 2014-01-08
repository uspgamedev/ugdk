#include <ugdk/debug/profiler.h>

#include <cassert>
#include "SDL_timer.h"

namespace ugdk {
namespace debug {

namespace {
ProfileSection* active_section = nullptr;
}

ProfileSection::ProfileSection(const std::string& section_name) 
    : parent_section_(active_section)
    , start_time_(SDL_GetPerformanceCounter())
    , data_(new ProfileData)

{
    data_->name = section_name;
    active_section = this;
}

ProfileSection::~ProfileSection() {
    uint64 end_time = SDL_GetPerformanceCounter();

    data_->duration_ = end_time - start_time_;
    
    if(active_section == this)
        active_section = parent_section_;

    if(parent_section_)
        parent_section_->data_->subdata.push_back(data_);
}

}  // namespace debug
}  // namespace ugdk
