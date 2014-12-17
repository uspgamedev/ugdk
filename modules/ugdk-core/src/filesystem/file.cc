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

int File::fgetc() {
    unsigned char c;
    if (Read(&c, 1) == 1)
        return static_cast<int>(c);
    else
        return EOF;
}

char* File::fgets(char* str, int count) {
    char *buf = str;    
    int chars_left = count;

    int ch;
    while (--chars_left > 0 && (ch = fgetc()) != EOF) {
        if ((*buf++ = ch) == '\n')  /* EOL */
            break;
    }
    *buf = '\0';
    if (ch == EOF && buf == str)
        return NULL;
    return str;
}

} // namespace filesystem
} // namespace ugdk
