#ifndef UGDK_RESOURCE_H_
#define UGDK_RESOURCE_H_

namespace ugdk {
namespace resource {

class ResourceContainerBase;

template<class T>
class ResourceContainer;

class ResourceManager;

template <class T, class Loader_T = T (*) (const std::string&)>
class GenericContainer;

}  // namespace resource
}  // namespace ugdk

#endif
