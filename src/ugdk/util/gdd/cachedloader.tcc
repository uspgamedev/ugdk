
#ifdef DEBUG
#include <cstdio>
#include <typeinfo>
#define TOSTRING(X) typeid(X).name()
#endif

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
T* CachedLoader<T>::Load(const string& gddfile_path) {
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
T* CachedLoader<T>::loadFromCache(const string& gddfile_path) {
    DataIterator it = cache_.find(gddfile_path);
    if (it == cache_.end()) return NULL;
    else return it->second;
}

template <class T>
T* CachedLoader<T>::loadNewData(const string& gddfile_path) {
    T* new_data = SimpleLoader<T>::Load(gddfile_path);
    if (new_data) addToCache(gddfile_path, new_data);
    return new_data;
}

template <class T>
void CachedLoader<T>::addToCache(const string& gddfile_path, T*& new_data) {
    cache_[gddfile_path] = new_data;
}

/// ResourceContainer methods
template <class T>
void CachedLoader<T>::Insert( const std::string& tag, T* val) {
#ifdef DEBUG
    fprintf(stderr, "UGDK::CachedLoader<%s> Warning - Method 'Insert' should not be used.", TOSTRING(T));
#endif
}

template <class T>
void CachedLoader<T>::Replace(const std::string& tag, T* val) {
#ifdef DEBUG
    fprintf(stderr, "UGDK::CachedLoader<%s> Warning - Method 'Replace' should not be used.", TOSTRING(T));
#endif
}

template <class T>
bool CachedLoader<T>::Exists( const std::string& tag) const {
    typename DataMap::const_iterator it = cache_.find(tag);
    return it != cache_.end();
}

template <class T>
T*   CachedLoader<T>::Find(   const std::string& tag) {
    return loadFromCache(tag);
}

template <class T>
T*   CachedLoader<T>::Load(   const std::string& filepath, const std::string& tag) {
    return Load(filepath);
}

} /* namespace gdd */

} /* namespace ugdk */
