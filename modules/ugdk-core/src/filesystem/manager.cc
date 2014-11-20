#include <ugdk/filesystem/manager.h>

namespace ugdk {
namespace filesystem {

// Both are defined in .cc because internal::SDLEventHandler is only known here.
Manager::Manager() {}
Manager::~Manager() {}

bool Manager::Initialize() {
    return true;
}

void Manager::Release() {
}

}  // namespace filesystem
}  // namespace ugdk
