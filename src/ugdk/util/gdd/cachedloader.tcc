
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
    T* cached_data = loadFromCache(gddfile_path);
    if (cached_data) return cached_data;
    return loadNewData(gddfile_path);
}

template <class T>
void CachedLoader<T>::ClearCache() {
    DataIterator it;
    it = cache_.begin();
    for (it  = cache_.begin(); it != cache_.end(); ++it) {
        delete it->second;
    }
    cache_.clear();
}

template <class T>
T* CachedLoader<T>::loadFromCache(string& gddfile_path) {
    DataIterator it = cache_.find(gddfile_path);
    if (it == cache_.end()) return NULL;
    else return it->second;
}

template <class T>
T* CachedLoader<T>::loadNewData(string& gddfile_path) {
    T* new_data = SimpleLoader<T>::Load(gddfile_path);
    if (new_data) addToCache(gddfile_path, new_data);
    return new_data;
}

template <class T>
void CachedLoader<T>::addToCache(string& gddfile_path, T*& new_data) {
    cache_[gddfile_path] = new_data;
}

} /* namespace gdd */

} /* namespace ugdk */
