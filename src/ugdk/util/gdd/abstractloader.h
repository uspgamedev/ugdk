
#ifndef UGDK_UTIL_GDD_ABSTRACTLOADER_H_
#define UGDK_UTIL_GDD_ABSTRACTLOADER_H_

#include <ugdk/util/gdd/loaderinterface.h>

namespace ugdk {

namespace gdd {

template <class T>
class AbstractLoader : public LoaderInterface {

  public:

    virtual ~AbstractLoader() {}

    virtual T* Load(std::string gddfile_path) = 0;

  protected:

    AbstractLoader() {}

};

} /* namespace gdd */

} /* namespace ugdk */

#endif /* UGDK_UTIL_GDD_ABSTRACTLOADER_H_ */
