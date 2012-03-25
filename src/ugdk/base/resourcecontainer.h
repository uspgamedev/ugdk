#ifndef UGDK_BASE_RESOURCECONTAINER_H_
#define UGDK_BASE_RESOURCECONTAINER_H_

#include <string>

namespace ugdk {
namespace base {

template <class T>
class ResourceContainer {
  protected: ResourceContainer() {}
  public:
    virtual ~ResourceContainer() {}

    virtual void Insert( const std::string& tag, T val) = 0;
    virtual void Replace(const std::string& tag, T val) = 0;
    virtual bool Exists( const std::string& tag) const = 0;
    virtual T&   Find(   const std::string& tag) = 0;
    virtual T&   Load(   const std::string& filepath, const std::string& tag) = 0;
};

template <class T>
class ResourceContainer<T*> {
  protected: ResourceContainer() {}
  public:
    virtual ~ResourceContainer() {}

    virtual void Insert( const std::string& tag, T* val) = 0;
    virtual void Replace(const std::string& tag, T* val) = 0;
    virtual bool Exists( const std::string& tag) const = 0;
    virtual T*   Find(   const std::string& tag) = 0;
    virtual T*   Load(   const std::string& filepath, const std::string& tag) = 0;
};

} // namespace base
} // namespace ugdk

#endif // UGDK_BASE_RESOURCECONTAINER_H_

