#ifndef UGDK_UTIL_DESCRIPTIONPROTOCOL_H_
#define UGDK_UTIL_DESCRIPTIONPROTOCOL_H_

#include <ugdk/util/gdd/types.h>
#include <ugdk/util/gdd/descriptionprotocolbase.h>

namespace ugdk {
namespace gdd {

template <class T>
class DescriptionProtocol : public DescriptionProtocolBase {
  public:
    virtual ~DescriptionProtocol() {}

    virtual bool NewDescription() = 0;
    virtual bool NewData(const GDDString& data_name) = 0;
    virtual T* FinalizeDescription() = 0;

  protected:
    DescriptionProtocol() {}

};

} /* namespace gdd */
} /* namespace ugdk */

#endif /* UGDK_UTIL_DESCRIPTIONPROTOCOL_H_ */
