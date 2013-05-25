#include <ugdk/resource/manager.h>

static bool type_info_cmp(const std::type_info* a, const std::type_info* b) {
    return (a->before(*b) != 0);
}

namespace ugdk {
namespace resource {

Manager::Manager() : containers_(type_info_cmp) {}

Manager::~Manager() {
    for(const auto& it : containers_)
        delete it.second;
}

} // namespace resource
} // namespace ugdk
