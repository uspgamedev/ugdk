
#include <ugdk/util/gdd/parser.th>
#include <ugdk/util/gdd/simpleloader.h>
#include <ugdk/util/gdd/cachedloader.h>

namespace ugdk {

namespace gdd {

using std::string;

template <class T>
ParseStatus Parser<T>::Parse(string gddfile_path) {

    return ParseStatus::OK;

}

} /* namespace gdd */

} /* namespace ugdk */
