#ifndef UGDK_UTIL_GDD_TYPES_H_
#define UGDK_UTIL_GDD_TYPES_H_

#include <string>
#include <vector>

namespace ugdk {
namespace gdd {

typedef std::string               GDDString;
typedef std::vector<GDDString>    GDDArgs;

class LoadError {
  public:
    enum Type {
        TYPE_MISMATCH,
        INVALID_VALUE
    };
};

} /* namespace gdd */
} /* namespace ugdk */

#endif // UGDK_UTIL_GDD_TYPES_H_
