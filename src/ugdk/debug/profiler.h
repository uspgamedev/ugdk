#ifndef UGDK_DEBUG_PROFILER_H_
#define UGDK_DEBUG_PROFILER_H_

#include <ugdk/structure/types.h>

#include <string>
#include <vector>
#include <memory>

namespace ugdk {
namespace debug {

struct ProfileData;
struct ProfileData {
    std::string name;
    uint64 duration_;
    std::vector< std::shared_ptr<ProfileData> > subdata;
};

class ProfileSection {
  public:
    ProfileSection(const std::string& section_name);
    ~ProfileSection();

    std::shared_ptr<const ProfileData> data() const { return data_; }

  private:
    ProfileSection* parent_section_;
    uint64 start_time_;
    std::shared_ptr<ProfileData> data_;
};

}  // namespace debug
}  // namespace ugdk

#endif // UGDK_DEBUG_PROFILER_H_
