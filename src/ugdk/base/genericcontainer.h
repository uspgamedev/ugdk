#ifndef UGDK_BASE_GENERICCONTAINER_H_
#define UGDK_BASE_GENERICCONTAINER_H_

#include <string>
#include <map>
#include <ugdk/base/resourcecontainer.h>

#ifdef DEBUG
#include <cstdio>
#include <typeinfo>
#define TOSTRING(X) typeid(X).name()
#endif

namespace ugdk {
namespace base {

template <class T, class Loader_T = T (*) (const std::string&)>
class GenericContainer : public virtual ResourceContainer<T> {
  private:
    typedef std::map<std::string, T>   DataMap;
    typedef typename DataMap::iterator DataIterator;
    DataMap database_;
    Loader_T loader_;

  public:
    GenericContainer() : loader_(T::LoadFromFile) {}
    GenericContainer(Loader_T loader) : loader_(loader) {}
    virtual ~GenericContainer() {}

    void Insert(const std::string& tag, T val) {
        DataIterator it = database_.find(tag);
        if(it == database_.end()) {
            database_[tag] = val;
        } else {
            #ifdef DEBUG
                fprintf(stderr, "UGDK::GenericContainer<%s> Error - Tag '%s' already exists.\n", TOSTRING(T), tag.c_str());
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
    
    /// Warning: GenericContainer doesn't know how to actually load an object.
    virtual T& Load(const std::string& filepath, const std::string& tag) {
        if(!Exists(tag)) Insert(tag, loader_(filepath));
        return Find(tag);
    }
};

template <class T>
class GenericContainer<T*, T* (*) (const std::string&)> : public virtual ResourceContainer<T*> {
  private:
    typedef std::map<std::string, T*>   DataMap;
    typedef typename DataMap::iterator  DataIterator;
    typedef T* (*Loader_T) (const std::string&);

    DataMap database_;
    Loader_T loader_;

  public:
    GenericContainer() : loader_(T::LoadFromFile) {}
    GenericContainer(Loader_T loader) : loader_(loader) {}
    virtual ~GenericContainer() {
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
                fprintf(stderr, "UGDK::GenericContainer<%s> Error - Tag '%s' already exists.\n", TOSTRING(T), tag.c_str());
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
        typename DataMap::const_iterator it = database_.find(tag);
        return (it != database_.end() && it->second);
    }

    T* Find(const std::string& tag) {
        return database_[tag];
    }
    
    /// Uses T::Load(const std::string&) in order to Load a new object.
    virtual T* Load(const std::string& filepath, const std::string& tag) {
        if(Exists(tag)) return Find(tag);
        T* obj = loader_(filepath);
        Insert(tag, obj);
        return obj;
    }
};

} // namespace base
} // namespace ugdk

#endif // UGDK_BASE_GENERICCONTAINER_H_

