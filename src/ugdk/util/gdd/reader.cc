#include <ugdk/util/gdd/reader.h>
#include <ugdk/system/engine.h>

namespace ugdk {
namespace gdd {

Reader::Reader(const std::string &file_path) : file_path_(file_path), file_(nullptr), line_(1) {
    file_path_ = ugdk::system::ResolvePath(file_path);
}

}
}
