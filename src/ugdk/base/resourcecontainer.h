#ifndef UGDK_BASE_RESOURCECONTAINER_H_
#define UGDK_BASE_RESOURCECONTAINER_H_

#include <string>
#include <map>
#ifdef DEBUG
#include <cstdio>
#include <typeinfo>
#define TOSTRING(X) typeid(X).name()
#endif

namespace ugdk {
namespace base {

template <class T>
class ResourceContainer {
  private:
	typedef std::map<std::string, T>   DataMap;
	typedef typename DataMap::iterator DataIterator;
    DataMap database_;

  public:
    ResourceContainer() {}
    ~ResourceContainer() {}

    void Insert(const std::string& tag, T val) {
        DataIterator it = database_.find(tag);
        if(it == database_.end()) {
            database_[tag] = val;
        } else {
            #ifdef DEBUG
                fprintf(stderr, "UGDK::ResourceContainer<%s> Error - Tag '%s' already exists.\n", TOSTRING(T), tag.c_str());
            #endif
        }
    }

    void Replace(const std::string& tag, T val) {
        database_[tag] = val;
    }

    bool Exists(const std::string& tag) const {
        return database_.count(tag) > 0;
    }

    T& Find(const std::string& tag) {
        return database_[tag];
    }
};

template <class T>
class ResourceContainer<T*> {
  private:
	typedef std::map<std::string, T*>  DataMap;
	typedef typename DataMap::iterator DataIterator;
    DataMap database_;

  public:
    ResourceContainer() {}
    ~ResourceContainer() {
		DataIterator it;
        for(it = database_.begin(); it != database_.end(); ++it) {
            delete it->second;
        }
    }

    void Insert(const std::string& tag, T* val) {
        DataIterator it = database_.find(tag);
        if(it == database_.end() || it->second == NULL) {
            database_[tag] = val;
        } else {
            #ifdef DEBUG
                fprintf(stderr, "UGDK::ResourceContainer<%s> Error - Tag '%s' already exists.\n", TOSTRING(T), tag.c_str());
            #endif
        }
    }

    void Replace(const std::string& tag, T* val) {
        DataIterator it = database_.find(tag);
        if(it == database_.end() || it->second == NULL) {
            database_[tag] = val;
        } else {
            delete it->second;
            it->second = val;
        }
    }
    
    bool Exists(const std::string& tag) const {
        return database_.count(tag) > 0;
    }

    T* Find(const std::string& tag) {
        return database_[tag];
    }
};

} // namespace base
} // namespace ugdk

#endif // UGDK_BASE_RESOURCECONTAINER_H_

