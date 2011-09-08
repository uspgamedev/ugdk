
#include <ugdk/util/gdd/simpleloader.th>
#include <ugdk/util/gdd/parser.h>

namespace ugdk {

namespace gdd {

using std::string;

template <class T>
T* SimpleLoader<T>::Load(string gddfile_path) {
    Parser<T> parser(protocol());
    switch(parser.Parse(gddfile_path)) {
      case ParseStatus::OK:
        return loaded_data_;
        break;
      case ParseStatus::FILE_NOT_FOUND:
      case ParseStatus::SYNTAX_ERROR:
        break;
    }
    return NULL;
}

} /* namespace gdd */

} /* namespace ugdk */
