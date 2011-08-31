

#ifndef UGDK_UTIL_LOADERINTERFACE_H_
#define UGDK_UTIL_LOADERINTERFACE_H_

#include <string>
#include <vector>

namespace ugdk {

namespace gdd {

typedef const std::string               GDDString;
typedef const std::vector<GDDString>    GDDArgs;

class LoaderInterface {

  public:

    virtual ~LoaderInterface() {}

    typedef enum {
        OK, TYPE_MISMATCH, INVALID_VALUE
    } LoadStatus;

    virtual LoadStatus NewData(GDDString& data_name) = 0;

    virtual LoadStatus NewProperty(GDDString& property_name, GDDArgs& property_args) = 0;

    virtual LoadStatus NewSegment(GDDString& segment_typename);

    virtual LoadStatus NewEntry(GDDString& entry_name, GDDArgs& entry_args) = 0;

    virtual LoadStatus NewSimpleSegment(GDDString* segment_typename, GDDArgs& segment_args) = 0;

  protected:

    LoaderInterface() {}

};

} /* namespace gdd */

} /* namespace ugdk */

#endif /* UGDK_UTIL_LOADERINTERFACE_H_ */
