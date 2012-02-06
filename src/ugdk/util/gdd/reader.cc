#include <ugdk/util/gdd/reader.h>
#include <ugdk/util/pathmanager.h>
#include <ugdk/base/engine.h>

namespace ugdk {
namespace gdd {

Reader::Reader(std::string &file_path) : file_path_(file_path), file_(NULL), line_(1) {
    file_path_ = PATH_MANAGER()->ResolvePath(file_path);
}

}
}
