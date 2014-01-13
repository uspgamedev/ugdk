#ifndef UGDK_DEBUG_PROFILER_H_
#define UGDK_DEBUG_PROFILER_H_

#include <ugdk/structure/types.h>

#include <string>
#include <vector>
#include <memory>

namespace ugdk {
namespace debug {

struct SectionData;
struct SectionData {

    /// Name of this section.
    std::string name;

    /// Length of this section, in microseconds.
    uint64 duration_;
    std::vector< std::shared_ptr<SectionData> > subdata;
};

class ProfileSection {
  public:
    ProfileSection(const std::string& section_name);
    ~ProfileSection();

    std::shared_ptr<const SectionData> data() const { return data_; }

  private:
    ProfileSection* parent_section_;
    uint64 start_time_;
    std::shared_ptr<SectionData> data_;
};

}  // namespace debug
}  // namespace ugdk

#endif // UGDK_DEBUG_PROFILER_H_
