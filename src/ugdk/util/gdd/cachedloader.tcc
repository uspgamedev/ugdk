
#include <ugdk/util/gdd/cachedloader.th>
#include <ugdk/util/gdd/parser.h>

namespace ugdk {

namespace gdd {

using std::string;

template <class T>
T* CachedLoader<T>::Load(string gddfile_path) {
    Parser parser;
    switch(parser.Parse(gddfile_path, this)) {
      case Parser::OK:
        break;
      case Parser::NOT_FOUND:
      case Parser::SYNTAX_ERROR:
        break;
    }
    return NULL;
}

} /* namespace gdd */

} /* namespace ugdk */
