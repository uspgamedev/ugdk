#ifndef UGDK_UTIL_DESCRIPTIONPROTOCOL_H_
#define UGDK_UTIL_DESCRIPTIONPROTOCOL_H_

#include <ugdk/util/gdd/descriptionprotocolbase.h>

namespace ugdk {
namespace gdd {

template <class T>
class DescriptionProtocol : public DescriptionProtocolBase {
  public:
    virtual ~DescriptionProtocol() {}

    void set_loader(AbstractLoader<T> *loader) { loader_ = loader; }
    AbstractLoader<T>* loader() const { return loader_; }

    virtual bool NewDescription() = 0;
    virtual bool NewData(const GDDString& data_name) = 0;

  protected:
    DescriptionProtocol() : loader_(NULL) {}

  private:
    AbstractLoader<T>   *loader_;

};

} /* namespace gdd */
} /* namespace ugdk */

#endif /* UGDK_UTIL_DESCRIPTIONPROTOCOL_H_ */
