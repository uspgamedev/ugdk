
#include <ugdk/util/gdd/cachedloader.th>
#include <ugdk/util/gdd/parser.h>

namespace ugdk {

namespace gdd {

using std::string;
using std::map;

template <class T>
CachedLoader<T>::~CachedLoader() {
    ClearCache();
}

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

template <class T>
void CachedLoader<T>::ClearCache() {
    cache_.clear();
    /*DataMap::iterator it;
    it = cache_.begin();
    for (it  = cache_.begin(); it != cache_.end(); ++it) {
        delete it->second.ref_;
    }
    */
}

} /* namespace gdd */

} /* namespace ugdk */
