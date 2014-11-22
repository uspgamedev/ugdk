#include <ugdk/filesystem/manager.h>

#include <ugdk/system/compatibility.h>
#include <filesystem/sdlfile.h>

#include "SDL_rwops.h"

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

void Manager::AddSearchPath(const std::string& path) {
    search_paths_.push_back(path);
    if (search_paths_.back().back() != '/')
        search_paths_.back().append("/");
}

std::unique_ptr<File> Manager::OpenFile(const std::string& path, const char* mode) const {
    for (const auto& search : search_paths_) {
        std::string fullpath = search + path;
        auto file = SDL_RWFromFile(fullpath.c_str(), mode);
        if (file)
            return MakeUnique<SDLFile>(file);
    }
    return std::unique_ptr<File>();
}
    
}  // namespace filesystem
}  // namespace ugdk
