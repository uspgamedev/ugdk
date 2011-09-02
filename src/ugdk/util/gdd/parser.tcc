
#include <ugdk/util/gdd/parser.th>
#include <ugdk/util/gdd/simpleloader.h>
#include <ugdk/util/gdd/cachedloader.h>

namespace ugdk {

namespace gdd {

using std::string;

template <class T>
ParseStatus Parser<T>::Parse(string gddfile_path, LoaderProtocol<T>* loader) {

    return PARSE_OK;

}

} /* namespace gdd */

} /* namespace ugdk */
