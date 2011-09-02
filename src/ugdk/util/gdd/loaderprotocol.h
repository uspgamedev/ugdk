

#ifndef UGDK_UTIL_LOADERINTERFACE_H_
#define UGDK_UTIL_LOADERINTERFACE_H_

#include <string>
#include <vector>
#include <ugdk/util/gdd/abstractloader.h>

namespace ugdk {

namespace gdd {

typedef const std::string               GDDString;
typedef const std::vector<GDDString>    GDDArgs;

typedef enum {
    LOAD_OK,
    LOAD_ERROR_TYPE_MISMATCH,
    LOAD_ERROR_INVALID_VALUE
} LoadStatus;

template <class T>
class LoaderProtocol {

  public:

    virtual ~LoaderProtocol() {}

    void set_loader(AbstractLoader<T> *loader) { loader_ = loader; }

    AbstractLoader<T>*& loader() const { return loader_; }

    virtual LoadStatus NewData(GDDString& data_name) = 0;

    virtual LoadStatus NewProperty(GDDString& property_name, GDDArgs& property_args) = 0;

    virtual LoadStatus NewSegment(GDDString& segment_typename);

    virtual LoadStatus NewEntry(GDDString& entry_name, GDDArgs& entry_args) = 0;

    virtual LoadStatus NewSimpleSegment(GDDString* segment_typename, GDDArgs& segment_args) = 0;

  protected:

    LoaderProtocol() : loader_(NULL) {}

  private:

    AbstractLoader<T> *loader_;

};

} /* namespace gdd */

} /* namespace ugdk */

#endif /* UGDK_UTIL_LOADERINTERFACE_H_ */
