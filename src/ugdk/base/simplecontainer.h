#ifndef UGDK_BASE_SIMPLECONTAINER_H_
#define UGDK_BASE_SIMPLECONTAINER_H_

#include <string>

namespace ugdk {
namespace base {

template <class T>
class SimpleContainer {
  protected: SimpleContainer() {}
  public:
    virtual ~SimpleContainer() {}

    virtual void Insert( const std::string& tag, T val) = 0;
    virtual void Replace(const std::string& tag, T val) = 0;
    virtual bool Exists( const std::string& tag) const = 0;
    virtual T&   Find(   const std::string& tag) = 0;
};

template <class T>
class SimpleContainer<T*> {
  protected: SimpleContainer() {}
  public:
    virtual ~SimpleContainer() {}

    virtual void Insert( const std::string& tag, T* val) = 0;
    virtual void Replace(const std::string& tag, T* val) = 0;
    virtual bool Exists( const std::string& tag) const = 0;
    virtual T*   Find(   const std::string& tag) = 0;
};

} // namespace base
} // namespace ugdk

#endif // UGDK_BASE_SIMPLECONTAINER_H_

