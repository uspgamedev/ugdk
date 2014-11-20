#include <ugdk/filesystem/file.h>

namespace ugdk {
namespace filesystem {

int64 File::Tell() {
    return Seek(0, SeekPositions::CURRENT);
}

} // namespace filesystem
} // namespace ugdk
