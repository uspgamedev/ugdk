

#ifndef UGDK_UTIL_BASELOADER_H_
#define UGDK_UTIL_BASELOADER_H_

#include <string>
#include <vector>

namespace ugdk {

namespace gdd {

typedef const std::string               GDDString;
typedef const std::vector<GDDString>    GDDArguments;

class BaseLoader {

  public:

    ~BaseLoader() {}

    typedef enum {
        OK, TYPE_MISMATCH, INVALID_VALUE
    } LoadStatus;

    LoadStatus NewData(GDDString& data_name) = 0;

    LoadStatus NewProperty(GDDString& property_name, GDDArguments& property_args) = 0;

    LoadStatus NewSegment(GDDString& segment_typename);

    LoadStatus NewEntry(GDDString& entry_name, GDDArguments& entry_args) = 0;

    LoadStatus NewSimpleSegment(GDDString* segment_typename, GDDArguments& segment_args) = 0;

  protected:

    BaseLoader() {}

};

} /* namespace gdd */

} /* namespace ugdk */

#endif /* UGDK_UTIL_BASELOADER_H_ */
