
#include <ugdk/util/gdd/simpleloader.th>
#include <ugdk/util/gdd/parser.h>

namespace ugdk {

namespace gdd {

using std::string;

template <class T>
T* SimpleLoader<T>::Load(string gddfile_path) {
    Parser<T> parser(protocol());
    switch(parser.Parse(gddfile_path, this)) {
      case PARSE_OK:
        return loaded_data_;
        break;
      case PARSE_FILE_NOT_FOUND:
      case PARSE_SYNTAX_ERROR:
        break;
    }
    return NULL;
}

} /* namespace gdd */

} /* namespace ugdk */
