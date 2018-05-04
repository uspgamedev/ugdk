#ifndef UGDK_RESOURCE_RESOURCECONTAINER_H_
#define UGDK_RESOURCE_RESOURCECONTAINER_H_

#include <string>
#include <memory>
#include <unordered_map>

#ifdef DEBUG
#include <cstdio>
#include <typeinfo>
#define TOSTRING(X) typeid(X).name()
#else
#define TOSTRING(X) "<UNKTYPE>"
#endif

#include <ugdk/system/config.h>
#include <ugdk/debug/log.h>

namespace ugdk {
namespace resource {

template <class T>
class ResourceContainer {
  public:
    static ResourceContainer* Get() {
        return Storage().get();
    }
    static void Clear() {
        auto storage = std::move(ResourceContainer<T>::Storage());
        if (storage) {
            auto &map = storage->database_;
            for (auto pair : map)
                delete pair.second;
            Storage().reset();
        }
    }
    template<class ...Args>
    static void Create(Args... args) {
        Storage().reset(new ResourceContainer(std::forward<Args>(args)...));
    }

    ~ResourceContainer() {
        ResourceContainer<T>::Clear();
    }

    void Insert(const std::string& tag, T* val);
    void Replace(const std::string& tag, T* val);
    bool Exists(const std::string& tag) const;
    T*   Find(const std::string& tag);
    T*   Load(const std::string& filepath, const std::string& tag);

  protected:
    static std::unique_ptr<ResourceContainer>& Storage() {
        static std::unique_ptr<ResourceContainer> ptr;
        return ptr;
    }

  private:
    using DataMap = std::unordered_map < std::string, T* >;
    using Loader_T = T* (*)(const std::string&);

    DataMap database_;
    Loader_T loader_;

    ResourceContainer(Loader_T loader) : loader_(loader) {}
};

template<class T>
void ResourceContainer<T>::Insert(const std::string& tag, T* val) {
    auto& pos = database_[tag];
    debug::DebugConditionalLog(!pos, debug::LogLevel::ERROR, "UGDK",
                               "ResourceContainer<", TOSTRING(T), "> - Tag '", tag, "' already exists.");
    pos = val;
}

template<class T>
void ResourceContainer<T>::Replace(const std::string& tag, T* val) {
    delete database_[tag];
    database_[tag] = val;
}

template<class T>
bool ResourceContainer<T>::Exists(const std::string& tag) const {
    typename DataMap::const_iterator it = database_.find(tag);
    return (it != database_.end() && it->second);
}

template<class T>
T* ResourceContainer<T>::Find(const std::string& tag) {
    return database_[tag];
}

/// Uses T::Load(const std::string&) in order to Load a new object.
template<class T>
T* ResourceContainer<T>::Load(const std::string& filepath, const std::string& tag) {
    if (Exists(tag)) return Find(tag);
    T* obj = loader_(filepath);
    debug::DebugConditionalLog(obj != nullptr, debug::LogLevel::ERROR, "UGDK",
                                "GenericContainer<", TOSTRING(T), "> - loader_ for '", tag, "' returned nullptr.");
    Insert(tag, obj);
    return obj;
}

template <class T>
inline T* NullLoad(const std::string& filepath) {
    debug::DebugLog(debug::LogLevel::WARNING, "UGDK",
                    "NullLoad<", TOSTRING(T), "> called with path '", filepath, "'");
    return nullptr;
}

} // namespace resource
} // namespace ugdk

#endif // UGDK_RESOURCE_RESOURCECONTAINER_H_

