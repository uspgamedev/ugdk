#include <ugdk/resource/manager.h>

namespace ugdk {
namespace resource {

Manager::Manager() {}

Manager::~Manager() {
    for(const auto& it : containers_)
        delete it.second;
}

} // namespace resource
} // namespace ugdk
