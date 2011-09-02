
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

    virtual void newData(T* new_data) = 0;

    virtual T*& data() = 0;

    virtual void cleanData() = 0;

    AbstractLoader() {}

};

} /* namespace gdd */

} /* namespace ugdk */

#endif /* UGDK_UTIL_GDD_ABSTRACTLOADER_H_ */
