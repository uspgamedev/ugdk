#include <ugdk/filesystem/file.h>

#include <ugdk/system/exceptions.h>

namespace ugdk {
namespace filesystem {

int64 File::Tell() {
    return Seek(0, SeekPositions::CURRENT);
}
    
std::string File::GetContents() {
    if (!CanSeek())
        throw system::BaseException("Can't get file contents: file doesn't support seeking.");
    std::string contents;
    contents.resize(Seek(0, SeekPositions::END));
    Seek(0, SeekPositions::BEGINNING);
    Read(&contents[0], contents.size());
    return contents;
}
    
bool File::CanSeek() {
    return Seek(0, SeekPositions::CURRENT) != -1;
}

} // namespace filesystem
} // namespace ugdk
