#ifndef UGDK_BASE_LOADCONTAINER_H_
#define UGDK_BASE_LOADCONTAINER_H_

#include <ugdk/base/simplecontainer.h>

namespace ugdk {
namespace base {

template <class T>
class LoadContainer : public virtual SimpleContainer<T> {
  protected: LoadContainer() {}
  public:
    virtual ~LoadContainer() {}

    virtual T& Load(const std::string& filepath, const std::string& tag) = 0;
            T& Load(const std::string& filepath) { return Load(filepath, filepath); }
};

template <class T>
class LoadContainer<T*> : public virtual SimpleContainer<T*> {
  protected: LoadContainer() {}
  public:
    virtual ~LoadContainer() {}

    virtual T* Load(const std::string& filepath, const std::string& tag) = 0;
            T* Load(const std::string& filepath) { return Load(filepath, filepath); }
};

} // namespace base
} // namespace ugdk

#endif // UGDK_BASE_LOADCONTAINER_H_

