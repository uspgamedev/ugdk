

#ifndef UGDK_UTIL_DESCRIPTIONPROTOCOL_H_
#define UGDK_UTIL_DESCRIPTIONPROTOCOL_H_

#include <string>
#include <vector>
#include <ugdk/util/gdd/abstractloader.h>

namespace ugdk {

namespace gdd {

typedef const std::string               GDDString;
typedef const std::vector<GDDString>    GDDArgs;

class LoadStatus {
    enum Type {
        LOAD_OK,
        LOAD_ERROR_TYPE_MISMATCH,
        LOAD_ERROR_INVALID_VALUE
    };
};

template <class T>
class DescriptionProtocol {

  public:

    virtual ~DescriptionProtocol() {}

    void set_loader(AbstractLoader<T> *loader) { loader_ = loader; }

    AbstractLoader<T>*& loader() const { return loader_; }

    virtual LoadStatus::Type NewData(GDDString& data_name) = 0;

    virtual LoadStatus::Type NewProperty(GDDString& property_name, GDDArgs& property_args) = 0;

    virtual LoadStatus::Type NewSegment(GDDString& segment_typename);

    virtual LoadStatus::Type NewEntry(GDDString& entry_name, GDDArgs& entry_args) = 0;

    virtual LoadStatus::Type NewSimpleSegment(GDDString* segment_typename, GDDArgs& segment_args) = 0;

  protected:

    DescriptionProtocol() : loader_(NULL) {}

  private:

    AbstractLoader<T> *loader_;

};

} /* namespace gdd */

} /* namespace ugdk */

#endif /* UGDK_UTIL_DESCRIPTIONPROTOCOL_H_ */
