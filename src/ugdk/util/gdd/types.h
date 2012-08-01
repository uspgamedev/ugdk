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

class ArgsConverter {
public:
    virtual ~ArgsConverter() {}
    virtual bool Process(const GDDArgs& args) const = 0;

protected:
    ArgsConverter() {}
};

} /* namespace gdd */
} /* namespace ugdk */

#endif // UGDK_UTIL_GDD_TYPES_H_
