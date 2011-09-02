
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
T* CachedLoader<T>::Load(string& gddfile_path) {
    T* cached_data = loadFromCache(gddfile_path);
    if (cached_data) return cached_data;
    T* new_data = SimpleLoader<T>::Load(gddfile_path);
    if (new_data) addToCache(new_data);
    return new_data;
}

template <class T>
void CachedLoader<T>::ClearCache() {
    DataIterator it;
    it = cache_.begin();
    for (it  = cache_.begin(); it != cache_.end(); ++it) {
        delete it->second.ref_;
    }
    cache_.clear();
}



} /* namespace gdd */

} /* namespace ugdk */
