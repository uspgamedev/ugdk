#ifndef UGDK_FILESYSTEM_SDLFILE_H_
#define UGDK_FILESYSTEM_SDLFILE_H_

#include <ugdk/filesystem/file.h>

#include "SDL_rwops.h"

namespace ugdk {
namespace filesystem {

class SDLFile : public File {
  public:
    SDLFile(SDL_RWops* _rwops) : rwops_(_rwops) {}

    ~SDLFile() {
        if (rwops_)
            Close();
    }

    int64 size() const override {
        return SDL_RWsize(rwops_);
    }

    int64 Seek(int64 offset, SeekPositions whence) override {
        int w;
        switch (whence) {
            case SeekPositions::BEGINNING:
                w = RW_SEEK_SET;
                break;
            case SeekPositions::END:
                w = RW_SEEK_END;
                break;
            case SeekPositions::CURRENT:
            default:
                w = RW_SEEK_CUR;
                break;
        }
        return SDL_RWseek(rwops_, offset, w);
    }

    std::size_t ReadRaw(void* ptr, std::size_t size, std::size_t maxnum) override {
        return SDL_RWread(rwops_, ptr, size, maxnum);
    }

    std::size_t WriteRaw(const void* ptr, std::size_t size, std::size_t num) override {
        return SDL_RWwrite(rwops_, ptr, size, num);
    }

    bool Close() override {
        bool success = SDL_RWclose(rwops_) == 0;
        rwops_ = nullptr;
        return success;
    }

    SDL_RWops* rwops() { return rwops_; }

  private:
    SDL_RWops* rwops_;

};

} // namespace filesystem
} // namespace ugdk

#endif // UGDK_FILESYSTEM_SDLFILE_H_
